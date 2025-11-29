
#include <iostream>
#include <vector>
#include <utility>

void bubbleSort(std::vector<int>& v)
{
	int n = v.size();

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (v[j] > v[j + 1])

			{
				std::swap(v[j], v[j + 1]);
			}
		}
	}
}

int main()

{
	std::vector<int>v = { 3,2,9,1,6 };
	bubbleSort(v);

	std::cout << "Result:";

	for (int eleman : v)
	{
		std::cout << eleman << " ";

	}

	std::cout << std::endl;
	return 0;

}

