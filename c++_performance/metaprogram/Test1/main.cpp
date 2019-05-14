#include <iostream>
using namespace std;
static const int N = 4;
const static int ARRAY[N][N] = { {0, 1, 2, 3 }, {10, 11, 12, 13 }, {20, 21, 22, 23 }, {30, 31, 32, 33 } };
static int array[N][N] = { 0 };
static const int LOOP = 1000000;
void copy0() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			array[i][j] = ARRAY[i][j];
		}
	}
}
void copy1() {
	for (int i = 0; i < N; i++) {
		array[0][i] = ARRAY[0][i];
		array[1][i] = ARRAY[1][i];
		array[2][i] = ARRAY[2][i];
		array[3][i] = ARRAY[3][i];
	}
}
void show() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}
template<int dimension>
class Copy {
public:
	static void copy(int out[][dimension], const int in[][dimension]) {
		for (int i = 0; i < dimension; i++) {
			for (int j = 0; j < dimension; j++) {
				array[i][j] = ARRAY[i][j];
			}
		}
	}
};
int main() {
	for (int i = 0; i < LOOP; i++) {
		//copy0();		// 0.033s
		//copy1();		// 0.020s
		Copy<N>::copy(array, ARRAY);
	}

	return 0;
}