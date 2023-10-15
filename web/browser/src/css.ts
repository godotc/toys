export interface ISelector {
    tagName: string;
    id: string;
    class: string;
}

export interface IDeclaration {
    name: string;
    value: string | number;
}

export interface IRule {
    selectors: ISelector[];
    declarations: IDeclaration[];
}
