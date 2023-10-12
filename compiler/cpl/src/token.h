
#include <stdint.h>
#include <variant>
#include <vector>

#define BIT(x) 1 << X

enum class ETokenType : uint32_t {
  ILG = 1 << 0,

  OPERATOR,
  WORDS,
  NUM,
  LITERAL,

  KEYWORD,
  CONSTRAINT,
};

struct Token {
  std::variant<const char *, int, double> value;
  ETokenType type;
};

class Tokenizer {
public:
  enum class ETokenizerResult {

  };

  ETokenizerResult tokenizer(const char *source);

  std::vector<Token> tokens;
};
