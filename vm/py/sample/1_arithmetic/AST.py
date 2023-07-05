class AddNode(object):
    def __init__(self, left, right) -> None:
        self.left = left
        self.right = right

    def visit(self):
        lval = self.left.visit()
        rval = self.right.visit()
        #self.value = lval+rval
        # return self.value
        print("BINARY_ADD")


class SubNode(object):
    def __init__(self, left, right) -> None:
        self.left = left
        self.right = right

    def visit(self):
        lval = self.left.visit()
        rval = self.right.visit()
        #self.value = lval-rval
        # return self.value
        print("BINARY_SUB")


class MulNode(object):
    def __init__(self, left, right) -> None:
        self.left = left
        self.right = right

    def visit(self):
        lval = self.left.visit()
        rval = self.right.visit()
        #self.value = lval*rval
        # return self.value
        print("BINARY_MUT")


class DivNode(object):
    def __init__(self, left, right) -> None:
        self.left = left
        self.right = right

    def visit(self):
        lval = self.left.visit()
        rval = self.right.visit()
        #self.value = lval/rval
        # return self.value
        print("BINARY_DIV")


class ConstNode(object):
    def __init__(self, value):
        self.value = value

    def visit(self):
        # return self.value
        print(f"LOAD_CONST {self.value}")


def build_tree():
    a = ConstNode(12)
    b = ConstNode(48)
    c = ConstNode(59)

    nmul = MulNode(a, b)
    nadd = AddNode(nmul, c)

    return nadd


if __name__ == "__main__":
    root = build_tree()
    v = root.visit()
    print(v)
