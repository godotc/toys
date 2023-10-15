import { IDeclaration, IRule, ISelector } from "./css";
import { IElement, IText, INode, element, text } from "./node";

enum EParseErrorType {
    Element,
    Tag,
    CssSelector,
    CssDeclaration,
}

class ParseError extends Error {
    errorType: EParseErrorType;

    constructor(type: EParseErrorType, msg: string) {
        super(msg);
        this.errorType = type;
    }
}

export class Parser {
    rawText: string = "";
    len: number = 0;
    index: number = 0;
    stack: any[] = [];

    constructor() {}

    parse(source: string) {
        this.rawText = source.trim();
        this.len = this.rawText.length;
        this.index = 0;
        this.stack = [];

        const root = element("root");
        while (this.index < this.len) {
            if (this.rawText[this.index].startsWith("<")) {
                this.index++;
                this.parseElement(root);
            } else {
                this.parseText(root);
            }
        }
    }

    private parseRule() {
        const rule: IRule = {
            selectors: [],
            declarations: [],
        };

        rule.selectors = this.parseSelectors();
        rule.declarations = this.parseDeclartions();

        return rule;
    }

    private parseElement(parent: IElement) {
        const tag = this.parseTag();
        this.stack.push(tag);

        const elem = element(tag);

        parent.children.push(elem);
        this.parseAttributes(elem);

        while (this.index < this.len) {
            let num_spaces = this.removeSpaces();

            // no child tag
            if (!this.peekChar().startsWith("<")) {
                // this.parseTextNoTrim(elem, num_spaces);
                this.parseText(elem);
                continue;
            }

            this.eat(); // eat <
            this.removeSpaces(); // < ... tag

            // Tag end
            if (this.peekChar().startsWith("/")) {
                this.eat();

                const startTag = this.stack[this.stack.length - 1];

                const endTag = this.parseTag();
                if (startTag != endTag) {
                    throw new ParseError(
                        EParseErrorType.Tag,
                        `The end tag: ${endTag} did not match start tag: ${startTag}`
                    );
                }

                this.stack.pop();
                while (this.index < this.len && this.peekChar() !== ">") {
                    this.eat();
                }

                // end parse element
                break;
            }

            // child element
            else {
                this.parseElement(elem);
            }
        }

        this.eat(); // eat >
    }

    private parseText(parent: IElement) {
        let str = "";
        while (
            this.index < this.len &&
            !(
                this.peekChar() === "<" &&
                /\w|\//.test(this.rawText[this.index + 1])
            )
        ) {
            str += this.readChar();
        }

        // this.sliceText();
        // TODO: should the text in tag remain the text?  like sting in tag: "<a>  abadf fadf a adfadf</a>"
        parent.children.push(text(removeExtraSpaces(str)));
    }

    private parseTextNoTrim(parent: IElement, num_previous_space: number) {
        let str = " ".repeat(num_previous_space);
        while (this.index < this.len && this.peekChar() !== "<") {
            str += this.readChar();
        }
        parent.children.push(text(str));
    }

    private parseTag() {
        // let tagName = this.readString();
        // if (tagName.length == 0) {
        //     throw new ParseError(
        //         EParseErrorType.Element,
        //         "parse tag name failed"
        //     );
        // }

        let tag = "";
        this.removeSpaces();

        while (
            this.index < this.len &&
            this.rawText[this.index] !== " " &&
            this.rawText[this.index] !== ">"
        ) {
            tag += this.readChar();
        }

        if (tag.length == 0) {
            throw new ParseError(EParseErrorType.Tag, "Empty tag name");
        }

        this.sliceText();
        return tag;
    }

    private parseAttributes(root: IElement) {
        // <div ... > all in tags.
        while (this.index < this.len && this.peekChar() !== ">") {
            this.removeSpaces();
            this.parseAttr(root);
            this.removeSpaces();
        }
        this.eat(); // >
    }

    private parseAttr(elem: IElement) {
        // single attri : class="foo"
        let attri = "",
            value = "";

        while (
            this.index < this.len &&
            this.peekChar() !== "=" &&
            this.peekChar() !== ">"
        ) {
            attri += this.readChar();
        }

        this.sliceText();

        attri = attri.trim();
        if (!attri.trim()) return; // length?

        this.eat(); // = | >

        let startSymbol = "";
        if (this.peekChar() == "'" || this.peekChar() == '"') {
            startSymbol = this.readChar();
        }

        while (this.index < this.len && this.peekChar() != startSymbol) {
            value += this.readChar();
        }

        this.eat(); // " | '

        elem.attributes[attri] = value.trim();

        this.sliceText();
    }

    private parseSelectors() {
        while (this.index < this.len) {
            this.removeSpaces();

            let ch = this.peekChar();
            if (ch === "{") {
                return;
            }

            let selector = this.parseSelector();

            //  TODO: need check?
            if (
                selector.class.length == 0 &&
                selector.id.length == 0 &&
                selector.tagName.length == 0
            ) {
                throw new ParseError(
                    EParseErrorType.CssSelector,
                    "Emepty selector match"
                );
            }
        }
    }
    private parseSelector() {
        const selector: ISelector = {
            id: "",
            class: "",
            tagName: "",
        };

        switch (this.readChar()) {
            case ".":
                this.eat();
                selector.class = this.parseIdentifier();
                break;
            case "#":
                this.eat();
                selector.id = this.parseIdentifier();
                break;
            case "*":
                this.eat();
                selector.tagName = "*";
                break;
            default:
                selector.tagName = this.parseIdentifier();
        }

        return selector;
    }
    private parseDeclartions() {}
    private parseDeclartion() {
        const decl: IDeclaration = { name: "", value: "" };
        this.removeSpaces();
        decl.name = this.parseIdentifier();
        this.removeSpaces();

        while (this.index < this.len && this.peekChar() != ":") {
            this.eat();
        }

        this.eat(); // :

        this.removeSpaces();
        decl.value = this.parseIdentifier();
        this.removeSpaces;
        return decl;
    }

    private parseIdentifier() {
        let res = "";
        while (
            this.index < this.len &&
            this.identifierRE.test(this.peekChar())
        ) {
            res += this.readChar();
        }
        this.sliceText();
        return res;
    }

    protected removeSpaces() {
        let t = this.index;
        while (
            this.index < this.len &&
            (this.rawText[this.index] === " " ||
                this.rawText[this.index] === "\n")
        ) {
            ++this.index;
        }
        let num_space = this.index - t;
        this.sliceText();
        return num_space;
    }

    protected sliceText() {
        this.rawText = this.rawText.slice(this.index);
        this.len = this.rawText.length;
        this.index = 0;
    }

    private readString() {
        let str = "";
        while (this.index < this.len) {
            const ch = this.rawText[this.index];
            if (ch === " " || ch === ">") break;
            str += ch;
            ++this.index;
        }
        return str;
    }

    protected readChar = () => this.rawText[this.index++];
    protected eat = () => ++this.index;
    protected peekChar = () => this.rawText[this.index];
}
