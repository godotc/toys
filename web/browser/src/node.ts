export enum ENodeType {
    Element = 1,
    Text = 3,
}

export interface IElement {
    tagName: string;
    attributes: Record<string, string>;
    children: INode[];
    nodeType: ENodeType.Element;
}

export interface IText {
    nodeValue: string;
    nodeType: ENodeType.Text;
}

export type INode = IElement | IText;

export function element(tagName: string) {
    return {
        tagName,
        attributes: {},
        children: [],
        nodeType: ENodeType.Element,
    } as IElement;
}

export function text(data: string) {
    return {
        nodeValue: data,
        nodeType: ENodeType.Text,
    } as IText;
}
