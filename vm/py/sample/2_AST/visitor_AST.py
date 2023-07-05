import struct
import hiop


# TODO Full fill this `ConstNode's Implementation`

class ConstNode():
    def __init__(self, val) -> None:
        self.value = val

    def accept(self, visitor):
        visitor.VisitConst(self)


class BinaryOperation():
    ADD = 0
    SUB = 1
    MUL = 2
    DIV = 3

    def __init__(self, op, left, right) -> None:
        self.op = op
        self.left = left
        self .right = right

    def accept(self, visitor):
        visitor.visitBinaryOp(self)


class Visitor():
    def __init__(self) -> None:
        pass

    def visitConst(self):
        pass

    def visitBinaryOp(self):
        pass


class PrintVisitor(Visitor):

    def visitBinaryOp(self, bin_node: BinaryOperation):
        bin_node.left.accept(self)
        bin_node.right.accept(self)

        if bin_node.op == BinaryOperation.ADD:
            print(" ADD")
        if bin_node.op == BinaryOperation.SUB:
            print(" SUB")
        if bin_node.op == BinaryOperation.MUL:
            print(" MUL")
        if bin_node.op == BinaryOperation.DIV:
            print(" DIV")


class EmitVisitor(Visitor):
    def __init__(self) -> None:
        self.codes = []

    def visitBinaryOp(self, bin_node: BinaryOperation):
        bin_node.left.accept(self)
        bin_node.right.accept(self)

        if bin_node.op == BinaryOperation.ADD:
            self.codes.append(struct.pack("B", BinaryOperation.ADD))
        if bin_node.op == BinaryOperation.SUB:
            self.codes.append(struct.pack("B", BinaryOperation.SUB))
        if bin_node.op == BinaryOperation.MUL:
            self.codes.append(struct.pack("B", BinaryOperation.MUL))
        if bin_node.op == BinaryOperation.DIV:
            self.codes.append(struct.pack("B", BinaryOperation.DIV))
