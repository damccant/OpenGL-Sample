class A
{
public:
	int a;
	A(int a) : a(a) {}
};

class B : public A
{
public:
	int b;
	B(int a, int b) : A(a), b(b) {}
};

class C : public B
{
public:
	int c;
	C(int a, int b, int c) : B(a, b), c(c) {}
};

int main(int argc, char** argv)
{
	return 0;
}