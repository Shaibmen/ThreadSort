#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class MergeSort {
public:
    void Sort(std::vector<int>& arr, int left, int right) {
        if (left < right) {
            int mid = (left + right) / 2;

            std::thread leftThread(&MergeSort::Sort, this, std::ref(arr), left, mid);
            std::thread rightThread(&MergeSort::Sort, this, std::ref(arr), mid + 1, right);

            leftThread.join();
            rightThread.join();

            Merge(arr, left, mid, right);
        }
    }

private:
    void Merge(std::vector<int>& arr, int left, int mid, int right) {
        int i = left;
        int j = mid + 1;
        int k = 0;

        std::vector<int> temp(right - left + 1);

        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k] = arr[i];
                i++;
            }
            else {
                temp[k] = arr[j];
                j++;
            }
            k++;
        }

        while (i <= mid) {
            temp[k] = arr[i];
            i++;
            k++;
        }

        while (j <= right) {
            temp[k] = arr[j];
            j++;
            k++;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        for (int i = 0; i < temp.size(); i++) {
            arr[left + i] = temp[i];
        }
    }

    std::mutex mutex_;
};

int main() {
    setlocale(LC_ALL, "rus");

    std::vector<int> arr(10);

    std::cout << "Введите 10 разных чисел для сортировки" << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cin >> arr[i];
    }

    MergeSort mergeSort;

    //лямбда функция auto - авто вывод типа переменной
    auto sortFunc = [&](std::vector<int>& arr) {
        mergeSort.Sort(arr, 0, arr.size() - 1);
        };

    std::thread mergeSortThread(sortFunc, std::ref(arr));

    mergeSortThread.join();

    for (int i = 0; i < 10; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
