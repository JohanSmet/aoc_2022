#include <iostream>
#include <string>

using namespace std;

int main() {
	int total = 0;

	int l1, u1, l2, u2;
	char s1, s2, s3;

	while (cin >> l1 >> s1 >> u1 >> s2 >> l2 >> s3 >> u2) {
		if ((l1 >= l2 && u1 <= u2) || (l2 >= l1 && u2 <= u1)) {
			total += 1;
		}
	}

	cout << total << endl;
	return 0;
}
