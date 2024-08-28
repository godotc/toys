# Summary
# encode and decode prompt input to reduce cost of model computing power?

import unicodedata


def get_consecutive_pair_count(
    ids: list[int], counts=None
) -> dict[tuple[int, int], int] | dict:
    """
    Given a list of integers, return a dictionary of counts of consecutive pairs
    Example: [1, 2, 3, 1, 2] -> {(1, 2): 2, (2, 3): 1, (3, 1): 1}
    Optionally allows to update an existing dictionary of counts
    """
    counts = counts if counts else {}
    for pair in zip(ids, ids[1:]):
        counts[pair] = counts.get(pair, 0) + 1
    return counts


def merge_consecutive_pairs(ids, pair, idx):
    """
    In the list of integers (ids), replace all consecutive occurrences
    of pair with the new integer token idx
    Example: ids=[1, 2, 3, 1, 2], pair=(1, 2), idx=4 -> [4, 3, 4]
    """
    new_ids = []
    i = 0
    while i < len(ids):
        # if not at the very last position AND the pair matches, replace it
        if ids[i] == pair[0] and i < len(ids) - 1 and ids[i + 1] == pair[1]:
            new_ids.append(idx)
            i += 2
        else:
            new_ids.append(ids[i])
            i += 1
    return new_ids


def replace_control_characters(s: str) -> str:
    # we don't want to print control characters
    # which distort the output (e.g. \n or much worse)
    # https://stackoverflow.com/questions/4324790/removing-control-characters-from-a-string-in-python/19016117#19016117
    # http://www.unicode.org/reports/tr44/#GC_Values_Table
    chars = []
    for ch in s:
        if unicodedata.category(ch)[0] != "C":
            chars.append(ch)  # this character is ok
        else:
            chars.append(f"\\u{ord(ch):04x}")  # escape
    return "".join(chars)


def render_token(t: bytes) -> str:
    # pretty print a token, escaping control characters
    s = t.decode("utf-8", errors="replace")
    s = replace_control_characters(s)
    return s


class Tokenizer:
    def __init__(self) -> None:
        self.pattern = ""
        self.merges: dict[dict[int, int], int] = {}
        self.special_tokens: dict[str, int] = {}
        self.vocab: dict[int, bytes] = self._build_vocab()
        pass

    def save(self, file_prefix):
        self_file = file_prefix + ".self"
        with open(self_file, "w") as f:
            # write the version, pattern and merges, that's all that's needed
            f.write("minbpe v1\n")
            f.write(f"{self.pattern}\n")
            # write the special tokens, first the number of them, then each one
            f.write(f"{len(self.special_tokens)}\n")
            for special, idx in self.special_tokens.items():
                f.write(f"{special} {idx}\n")
            # the merges dict
            for idx1, idx2 in self.merges:
                f.write(f"{idx1} {idx2}\n")
        # write the vocab: for the human to look at
        vocab_file = file_prefix + ".vocab"
        inverted_merges = {idx: pair for pair, idx in self.merges.items()}
        with open(vocab_file, "w", encoding="utf-8") as f:
            for idx, token in self.vocab.items():
                # note: many tokens may be partial utf-8 sequences
                # and cannot be decoded into valid strings. Here we're using
                # errors='replace' to replace them with the replacement char �.
                # this also means that we couldn't possibly use .vocab in load()
                # because decoding in this way is a lossy operation!
                s = render_token(token)
                # find the children of this token, if any
                if idx in inverted_merges:
                    # if this token has children, render it nicely as a merge
                    idx0, idx1 = inverted_merges[idx]
                    s0 = render_token(self.vocab[idx0])
                    s1 = render_token(self.vocab[idx1])
                    f.write(f"[{s0}][{s1}] -> [{s}] {idx}\n")
                else:
                    # otherwise this is leaf token, just print it
                    # (this should just be the first 256 tokens, the bytes)
                    f.write(f"[{s}] {idx}\n")

    def load(self, self_file):
        """Inverse of save() but only for the self file"""
        assert self_file.endswith(".self")
        # read the self file
        merges = {}
        special_tokens = {}
        idx = 256
        with open(self_file, "r", encoding="utf-8") as f:
            # read the version
            version = f.readline().strip()
            assert version == "minbpe v1"
            # read the pattern
            self.pattern = f.readline().strip()
            # read the special tokens
            num_special = int(f.readline().strip())
            for _ in range(num_special):
                special, special_idx = f.readline().strip().split()
                special_tokens[special] = int(special_idx)
            # read the merges
            for line in f:
                idx1, idx2 = map(int, line.split())
                merges[(idx1, idx2)] = idx
                idx += 1
        self.merges = merges
        self.special_tokens = special_tokens
        self.vocab = self._build_vocab()

    def _build_vocab(self):
            # vocab is simply and deterministically derived from merges
            vocab = {idx: bytes([idx]) for idx in range(256)}
            for (p0, p1), idx in self.merges.items():
                vocab[idx] = vocab[p0] + vocab[p1]
            for special, idx in self.special_tokens.items():
                vocab[idx] = special.encode("utf-8")
            return vocab


class BasicTokenizer(Tokenizer):
    def __init__(self) -> None:
        super().__init__()
        pass

    def train(self, text, vocab_size, verbose=True):
        assert vocab_size >= 256
        num_merges = vocab_size - 256

        text_bytes = text.encode("utf-8")
        ids = list(text_bytes)

        new_merges = {}
        new_vocab = {idx: bytes([idx]) for idx in range(256)}

        for i in range(num_merges):
            pair_counts = get_consecutive_pair_count(ids)

            # get the most frequent pair
            pair = max(pair_counts, key=lambda x: pair_counts.get(x, 0))

            # add new pair as a index into the merges and vocab
            idx = 256 + i
            ids = merge_consecutive_pairs(ids, pair, idx)
            new_merges[pair] = idx
            new_vocab[idx] = new_vocab[pair[0]] + new_vocab[pair[1]]

            if verbose:
                msg = f"merge {i+1}/{num_merges}:\n"
                msg += f"\t in byte: ({repr(chr(pair[0]))}, {repr(chr(pair[1]))})\n"
                msg += f"\t {pair} -> {idx}\n"
                msg += f"\t new_merges [ {pair} ] == {idx}\n"
                msg += f"\t new_vocab[ {idx} ] == {new_vocab[idx]}\n"
                msg += f"\t ({new_vocab[idx]}) had  {pair_counts[pair]} occurrences\n"
                print(msg)

        self.merges = new_merges
        self.vocab = new_vocab
        pass

    def encode(self, text):
        text_bytes = text.encode("utf-8")
        ids = list(text_bytes)
        while len(ids) >= 2:
            pair_counts = get_consecutive_pair_count(ids)
            pair = min(pair_counts, key=lambda x: self.merges.get(x, float("inf")))

            # the minimum pair not in merges, so we are done
            if pair not in self.merges:
                break

            idx = self.merges[pair]
            # redo merge to increase ids and then to  reduce the text length for encode
            ids = merge_consecutive_pairs(ids, pair, idx)

        return ids  # index sequence

    def decode(self, tokens):
        # tokens: int stream -> replace by vocab[token]
        # ids -> tokens -> text
        text_bytes = b"".join(self.vocab[token] for token in tokens)
        text = text_bytes.decode("utf-8", errors="replace")
        return text

  



GPT4_SPLIT_PATTERN = r"""'(?i:[sdmt]|ll|ve|re)|[^\r\n\p{L}\p{N}]?+\p{L}+|\p{N}{1,3}| ?[^\s\p{L}\p{N}]++[\r\n]*|\s*[\r\n]|\s+(?!\S)|\s+"""



def train_encode_decode(bBasic, vocab_size=256, verbose=True):
    tokenizer = BasicTokenizer() if bBasic else None
    with open("test/taylorswift.txt", "r", encoding="utf-8") as f:
        text = f.read()
        tokenizer.train(text, vocab_size, verbose)

        save(tokenizer, "basic_tokenizer")

        ids = tokenizer.encode(text)
        open("ids.txt", "wb").write(b"".join(bytes(i) for i in ids))

        # text_bytes = b"".join(tokenizer.vocab[token] for token in ids)
        text_bytes = bytes([id & 0xFF for id in ids])
        text = text_bytes.decode("utf-8", errors="replace")
        open("encoded_text.txt", "w", encoding="utf-8").write(text)

        text = tokenizer.decode(ids)
        open("decoded_text.txt", "w", encoding="utf-8").write(text)

        import os

        size0 = os.path.getsize("test/taylorswift.txt")
        size1 = os.path.getsize("encoded_text.txt")
        print(f"size0 = {size0}, size1 = {size1}, ratio = {size1 / size0}")


# Basic
# 266 size0 = 185768, size1 = 163165, ratio = 0.8783267301149821    exited with code=0 in 0.848 seconds
# 300 size0 = 185768, size1 = 130556, ratio = 0.7027905774945092   exited with code=0 in 2.692 seconds
# 500: size0 = 185768, size1 = 104411, ratio = 0.5620505146203867  exited with code=0 in 10.828 seconds

train_encode_decode(True, 266, False)
