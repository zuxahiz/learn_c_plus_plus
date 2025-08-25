# 排序算法详解

## 🎯 排序算法概述

排序是计算机科学中最基础也是最重要的算法之一。理解各种排序算法的原理、实现和性能特征，对于算法设计和优化具有重要意义。

### 排序算法分类
1. **比较排序**: 基于元素间比较的排序算法
2. **非比较排序**: 不基于比较的排序算法  
3. **内部排序**: 所有数据都在内存中
4. **外部排序**: 数据量大，需要使用外存

### 重要概念
- **稳定性**: 相等元素的相对位置是否改变
- **原地排序**: 是否需要额外的存储空间
- **自适应性**: 对已有序程度的敏感性

## 📚 1. 基础排序算法

### 1.1 冒泡排序 (Bubble Sort)

**算法思想**: 重复遍历数组，比较相邻元素并交换位置，使较大元素逐渐"冒泡"到数组末尾。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

template<typename T>
class BubbleSort {
public:
    // 基础冒泡排序
    static void basicBubbleSort(std::vector<T>& arr) {
        int n = arr.size();
        
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1 - i; j++) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
    
    // 优化版本1：提前终止
    static void optimizedBubbleSort(std::vector<T>& arr) {
        int n = arr.size();
        
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            
            for (int j = 0; j < n - 1 - i; j++) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            
            // 如果这一轮没有交换，说明已经有序
            if (!swapped) {
                break;
            }
        }
    }
    
    // 优化版本2：双向冒泡（鸡尾酒排序）
    static void cocktailSort(std::vector<T>& arr) {
        int left = 0;
        int right = arr.size() - 1;
        
        while (left < right) {
            // 从左到右冒泡最大值
            for (int i = left; i < right; i++) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                }
            }
            right--;
            
            // 从右到左冒泡最小值
            for (int i = right; i > left; i--) {
                if (arr[i] < arr[i - 1]) {
                    std::swap(arr[i], arr[i - 1]);
                }
            }
            left++;
        }
    }
    
    // 带计数器的冒泡排序（用于分析）
    static std::pair<int, int> bubbleSortWithStats(std::vector<T>& arr) {
        int n = arr.size();
        int comparisons = 0;
        int swaps = 0;
        
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1 - i; j++) {
                comparisons++;
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                    swaps++;
                }
            }
        }
        
        return {comparisons, swaps};
    }
};
```

**复杂度分析**:
- 时间复杂度: O(n²) 平均和最坏情况，O(n) 最好情况（已排序）
- 空间复杂度: O(1)
- 稳定性: 稳定
- 适用场景: 教学演示，小数据集

### 1.2 选择排序 (Selection Sort)

**算法思想**: 每次从未排序部分选择最小元素，放到已排序部分的末尾。

```cpp
template<typename T>
class SelectionSort {
public:
    // 基础选择排序
    static void basicSelectionSort(std::vector<T>& arr) {
        int n = arr.size();
        
        for (int i = 0; i < n - 1; i++) {
            int minIndex = i;
            
            // 找到最小元素的索引
            for (int j = i + 1; j < n; j++) {
                if (arr[j] < arr[minIndex]) {
                    minIndex = j;
                }
            }
            
            // 交换到正确位置
            if (minIndex != i) {
                std::swap(arr[i], arr[minIndex]);
            }
        }
    }
    
    // 双向选择排序
    static void doubleSelectionSort(std::vector<T>& arr) {
        int left = 0;
        int right = arr.size() - 1;
        
        while (left < right) {
            int minIndex = left;
            int maxIndex = left;
            
            // 同时找到最小和最大元素
            for (int i = left; i <= right; i++) {
                if (arr[i] < arr[minIndex]) {
                    minIndex = i;
                }
                if (arr[i] > arr[maxIndex]) {
                    maxIndex = i;
                }
            }
            
            // 将最小元素放到左边
            std::swap(arr[left], arr[minIndex]);
            
            // 如果最大元素在最小元素的原位置，需要调整索引
            if (maxIndex == left) {
                maxIndex = minIndex;
            }
            
            // 将最大元素放到右边
            std::swap(arr[right], arr[maxIndex]);
            
            left++;
            right--;
        }
    }
    
    // 堆选择排序（使用堆优化）
    static void heapSelectionSort(std::vector<T>& arr) {
        std::make_heap(arr.begin(), arr.end());
        
        for (int i = arr.size() - 1; i > 0; i--) {
            std::pop_heap(arr.begin(), arr.begin() + i + 1);
        }
    }
};
```

**复杂度分析**:
- 时间复杂度: O(n²) 所有情况
- 空间复杂度: O(1)
- 稳定性: 不稳定
- 特点: 交换次数最少 O(n)

### 1.3 插入排序 (Insertion Sort)

**算法思想**: 将数组分为已排序和未排序两部分，逐个将未排序元素插入到已排序部分的正确位置。

```cpp
template<typename T>
class InsertionSort {
public:
    // 基础插入排序
    static void basicInsertionSort(std::vector<T>& arr) {
        int n = arr.size();
        
        for (int i = 1; i < n; i++) {
            T key = arr[i];
            int j = i - 1;
            
            // 将大于key的元素向右移动
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            
            arr[j + 1] = key;
        }
    }
    
    // 二分插入排序
    static void binaryInsertionSort(std::vector<T>& arr) {
        int n = arr.size();
        
        for (int i = 1; i < n; i++) {
            T key = arr[i];
            
            // 使用二分查找找到插入位置
            int left = 0;
            int right = i;
            
            while (left < right) {
                int mid = left + (right - left) / 2;
                if (arr[mid] <= key) {
                    left = mid + 1;
                } else {
                    right = mid;
                }
            }
            
            // 移动元素
            for (int j = i; j > left; j--) {
                arr[j] = arr[j - 1];
            }
            
            arr[left] = key;
        }
    }
    
    // 希尔排序（插入排序的改进版）
    static void shellSort(std::vector<T>& arr) {
        int n = arr.size();
        
        // 使用Knuth间隔序列: 1, 4, 13, 40, 121, ...
        for (int gap = 1; gap < n / 3; gap = gap * 3 + 1);
        
        while (gap >= 1) {
            // 对每个间隔进行插入排序
            for (int i = gap; i < n; i++) {
                T key = arr[i];
                int j = i;
                
                while (j >= gap && arr[j - gap] > key) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                
                arr[j] = key;
            }
            
            gap /= 3;
        }
    }
    
    // 链表插入排序（用于链表结构）
    template<typename Node>
    static Node* linkedListInsertionSort(Node* head) {
        if (!head || !head->next) return head;
        
        Node* dummy = new Node(0);
        Node* current = head;
        
        while (current) {
            Node* next = current->next;
            
            // 在已排序部分找到插入位置
            Node* prev = dummy;
            while (prev->next && prev->next->data < current->data) {
                prev = prev->next;
            }
            
            // 插入当前节点
            current->next = prev->next;
            prev->next = current;
            
            current = next;
        }
        
        Node* result = dummy->next;
        delete dummy;
        return result;
    }
};
```

**复杂度分析**:
- 时间复杂度: O(n²) 平均和最坏情况，O(n) 最好情况
- 空间复杂度: O(1)
- 稳定性: 稳定
- 特点: 对小数据和近似有序数据效果好

## 📚 2. 高级排序算法

### 2.1 归并排序 (Merge Sort)

**算法思想**: 采用分治思想，将数组递归分割为更小的子数组，然后合并有序的子数组。

```cpp
template<typename T>
class MergeSort {
public:
    // 基础归并排序
    static void basicMergeSort(std::vector<T>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            
            basicMergeSort(arr, left, mid);
            basicMergeSort(arr, mid + 1, right);
            
            merge(arr, left, mid, right);
        }
    }
    
    // 包装函数
    static void mergeSort(std::vector<T>& arr) {
        basicMergeSort(arr, 0, arr.size() - 1);
    }
    
    // 迭代版归并排序
    static void iterativeMergeSort(std::vector<T>& arr) {
        int n = arr.size();
        
        for (int currSize = 1; currSize < n; currSize *= 2) {
            for (int leftStart = 0; leftStart < n - 1; leftStart += 2 * currSize) {
                int mid = std::min(leftStart + currSize - 1, n - 1);
                int rightEnd = std::min(leftStart + 2 * currSize - 1, n - 1);
                
                if (mid < rightEnd) {
                    merge(arr, leftStart, mid, rightEnd);
                }
            }
        }
    }
    
    // 原地归并排序（空间优化）
    static void inPlaceMergeSort(std::vector<T>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            
            inPlaceMergeSort(arr, left, mid);
            inPlaceMergeSort(arr, mid + 1, right);
            
            inPlaceMerge(arr, left, mid, right);
        }
    }
    
    // 三路归并排序
    static void threeWayMergeSort(std::vector<T>& arr, int left, int right) {
        if (right - left < 2) return;
        
        int mid1 = left + (right - left) / 3;
        int mid2 = left + 2 * (right - left) / 3;
        
        threeWayMergeSort(arr, left, mid1);
        threeWayMergeSort(arr, mid1, mid2);
        threeWayMergeSort(arr, mid2, right);
        
        threeWayMerge(arr, left, mid1, mid2, right);
    }
    
private:
    // 合并两个有序子数组
    static void merge(std::vector<T>& arr, int left, int mid, int right) {
        std::vector<T> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        
        // 合并过程
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        // 复制剩余元素
        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        while (j <= right) {
            temp[k++] = arr[j++];
        }
        
        // 复制回原数组
        for (int i = 0; i < k; i++) {
            arr[left + i] = temp[i];
        }
    }
    
    // 原地合并（Block Merge）
    static void inPlaceMerge(std::vector<T>& arr, int left, int mid, int right) {
        int start2 = mid + 1;
        
        // 如果已经有序，直接返回
        if (arr[mid] <= arr[start2]) {
            return;
        }
        
        while (left <= mid && start2 <= right) {
            if (arr[left] <= arr[start2]) {
                left++;
            } else {
                T value = arr[start2];
                int index = start2;
                
                // 移动元素
                while (index != left) {
                    arr[index] = arr[index - 1];
                    index--;
                }
                
                arr[left] = value;
                left++;
                mid++;
                start2++;
            }
        }
    }
    
    // 三路合并
    static void threeWayMerge(std::vector<T>& arr, int left, int mid1, int mid2, int right) {
        std::vector<T> temp(right - left);
        int i = left, j = mid1, k = mid2, l = 0;
        
        // 三路合并过程
        while (i < mid1 && j < mid2 && k < right) {
            if (arr[i] <= arr[j] && arr[i] <= arr[k]) {
                temp[l++] = arr[i++];
            } else if (arr[j] <= arr[i] && arr[j] <= arr[k]) {
                temp[l++] = arr[j++];
            } else {
                temp[l++] = arr[k++];
            }
        }
        
        // 处理剩余的两路合并
        while (i < mid1 && j < mid2) {
            if (arr[i] <= arr[j]) {
                temp[l++] = arr[i++];
            } else {
                temp[l++] = arr[j++];
            }
        }
        
        while (i < mid1 && k < right) {
            if (arr[i] <= arr[k]) {
                temp[l++] = arr[i++];
            } else {
                temp[l++] = arr[k++];
            }
        }
        
        while (j < mid2 && k < right) {
            if (arr[j] <= arr[k]) {
                temp[l++] = arr[j++];
            } else {
                temp[l++] = arr[k++];
            }
        }
        
        // 复制剩余元素
        while (i < mid1) temp[l++] = arr[i++];
        while (j < mid2) temp[l++] = arr[j++];
        while (k < right) temp[l++] = arr[k++];
        
        // 复制回原数组
        for (int i = 0; i < l; i++) {
            arr[left + i] = temp[i];
        }
    }
};
```

**复杂度分析**:
- 时间复杂度: O(n log n) 所有情况
- 空间复杂度: O(n)
- 稳定性: 稳定
- 特点: 性能稳定，适合大数据

### 2.2 快速排序 (Quick Sort)

**算法思想**: 选择一个基准元素，将数组分为小于和大于基准的两部分，然后递归排序两部分。

```cpp
template<typename T>
class QuickSort {
public:
    // 基础快速排序
    static void basicQuickSort(std::vector<T>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            
            basicQuickSort(arr, low, pi - 1);
            basicQuickSort(arr, pi + 1, high);
        }
    }
    
    // 包装函数
    static void quickSort(std::vector<T>& arr) {
        basicQuickSort(arr, 0, arr.size() - 1);
    }
    
    // 三路快速排序（处理重复元素）
    static void threeWayQuickSort(std::vector<T>& arr, int low, int high) {
        if (low < high) {
            auto [lt, gt] = threeWayPartition(arr, low, high);
            
            threeWayQuickSort(arr, low, lt - 1);
            threeWayQuickSort(arr, gt + 1, high);
        }
    }
    
    // 迭代版快速排序
    static void iterativeQuickSort(std::vector<T>& arr) {
        std::stack<std::pair<int, int>> stack;
        stack.push({0, static_cast<int>(arr.size()) - 1});
        
        while (!stack.empty()) {
            auto [low, high] = stack.top();
            stack.pop();
            
            if (low < high) {
                int pi = partition(arr, low, high);
                
                stack.push({low, pi - 1});
                stack.push({pi + 1, high});
            }
        }
    }
    
    // 混合排序（对小数组使用插入排序）
    static void hybridQuickSort(std::vector<T>& arr, int low, int high, int threshold = 10) {
        if (high - low <= threshold) {
            insertionSort(arr, low, high);
        } else if (low < high) {
            int pi = medianOfThreePartition(arr, low, high);
            
            hybridQuickSort(arr, low, pi - 1, threshold);
            hybridQuickSort(arr, pi + 1, high, threshold);
        }
    }
    
    // 尾递归优化
    static void tailRecursiveQuickSort(std::vector<T>& arr, int low, int high) {
        while (low < high) {
            int pi = partition(arr, low, high);
            
            // 总是先处理较小的部分
            if (pi - low < high - pi) {
                tailRecursiveQuickSort(arr, low, pi - 1);
                low = pi + 1;
            } else {
                tailRecursiveQuickSort(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
    
private:
    // Lomuto分割方案
    static int partition(std::vector<T>& arr, int low, int high) {
        T pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }
    
    // Hoare分割方案
    static int hoarePartition(std::vector<T>& arr, int low, int high) {
        T pivot = arr[low];
        int i = low - 1;
        int j = high + 1;
        
        while (true) {
            do {
                i++;
            } while (arr[i] < pivot);
            
            do {
                j--;
            } while (arr[j] > pivot);
            
            if (i >= j) {
                return j;
            }
            
            std::swap(arr[i], arr[j]);
        }
    }
    
    // 三路分割
    static std::pair<int, int> threeWayPartition(std::vector<T>& arr, int low, int high) {
        T pivot = arr[low];
        int lt = low;      // arr[low...lt-1] < pivot
        int i = low + 1;   // arr[lt...i-1] == pivot
        int gt = high + 1; // arr[gt...high] > pivot
        
        while (i < gt) {
            if (arr[i] < pivot) {
                std::swap(arr[lt++], arr[i++]);
            } else if (arr[i] > pivot) {
                std::swap(arr[i], arr[--gt]);
            } else {
                i++;
            }
        }
        
        return {lt, gt};
    }
    
    // 三数取中分割
    static int medianOfThreePartition(std::vector<T>& arr, int low, int high) {
        int mid = low + (high - low) / 2;
        
        // 排序三个元素
        if (arr[mid] < arr[low]) {
            std::swap(arr[low], arr[mid]);
        }
        if (arr[high] < arr[low]) {
            std::swap(arr[low], arr[high]);
        }
        if (arr[high] < arr[mid]) {
            std::swap(arr[mid], arr[high]);
        }
        
        // 将中位数放到末尾
        std::swap(arr[mid], arr[high]);
        
        return partition(arr, low, high);
    }
    
    // 插入排序（用于小数组）
    static void insertionSort(std::vector<T>& arr, int low, int high) {
        for (int i = low + 1; i <= high; i++) {
            T key = arr[i];
            int j = i - 1;
            
            while (j >= low && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            
            arr[j + 1] = key;
        }
    }
};
```

**复杂度分析**:
- 时间复杂度: O(n log n) 平均情况，O(n²) 最坏情况
- 空间复杂度: O(log n) 平均情况，O(n) 最坏情况
- 稳定性: 不稳定
- 特点: 实际性能优秀，原地排序

### 2.3 堆排序 (Heap Sort)

**算法思想**: 利用堆的性质进行排序，先建立最大堆，然后重复提取最大元素。

```cpp
template<typename T>
class HeapSort {
public:
    // 基础堆排序
    static void heapSort(std::vector<T>& arr) {
        int n = arr.size();
        
        // 建立最大堆
        buildMaxHeap(arr);
        
        // 一个个从堆顶取出元素
        for (int i = n - 1; i > 0; i--) {
            std::swap(arr[0], arr[i]);
            heapify(arr, 0, i);
        }
    }
    
    // 迭代版堆化
    static void iterativeHeapify(std::vector<T>& arr, int start, int end) {
        int parent = start;
        
        while (true) {
            int leftChild = 2 * parent + 1;
            int rightChild = 2 * parent + 2;
            int largest = parent;
            
            if (leftChild < end && arr[leftChild] > arr[largest]) {
                largest = leftChild;
            }
            
            if (rightChild < end && arr[rightChild] > arr[largest]) {
                largest = rightChild;
            }
            
            if (largest == parent) {
                break;
            }
            
            std::swap(arr[parent], arr[largest]);
            parent = largest;
        }
    }
    
    // 自底向上建堆
    static void buildMaxHeap(std::vector<T>& arr) {
        int n = arr.size();
        
        // 从最后一个非叶子节点开始
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, i, n);
        }
    }
    
    // 堆化过程
    static void heapify(std::vector<T>& arr, int root, int heapSize) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;
        
        if (left < heapSize && arr[left] > arr[largest]) {
            largest = left;
        }
        
        if (right < heapSize && arr[right] > arr[largest]) {
            largest = right;
        }
        
        if (largest != root) {
            std::swap(arr[root], arr[largest]);
            heapify(arr, largest, heapSize);
        }
    }
    
    // 基于优先队列的堆排序
    static void priorityQueueHeapSort(std::vector<T>& arr) {
        std::priority_queue<T> maxHeap(arr.begin(), arr.end());
        
        for (int i = arr.size() - 1; i >= 0; i--) {
            arr[i] = maxHeap.top();
            maxHeap.pop();
        }
    }
    
    // 最小堆排序（降序）
    static void minHeapSort(std::vector<T>& arr) {
        int n = arr.size();
        
        // 建立最小堆
        buildMinHeap(arr);
        
        // 一个个从堆顶取出元素
        for (int i = n - 1; i > 0; i--) {
            std::swap(arr[0], arr[i]);
            minHeapify(arr, 0, i);
        }
    }
    
private:
    static void buildMinHeap(std::vector<T>& arr) {
        int n = arr.size();
        
        for (int i = n / 2 - 1; i >= 0; i--) {
            minHeapify(arr, i, n);
        }
    }
    
    static void minHeapify(std::vector<T>& arr, int root, int heapSize) {
        int smallest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;
        
        if (left < heapSize && arr[left] < arr[smallest]) {
            smallest = left;
        }
        
        if (right < heapSize && arr[right] < arr[smallest]) {
            smallest = right;
        }
        
        if (smallest != root) {
            std::swap(arr[root], arr[smallest]);
            minHeapify(arr, smallest, heapSize);
        }
    }
};
```

**复杂度分析**:
- 时间复杂度: O(n log n) 所有情况
- 空间复杂度: O(1)
- 稳定性: 不稳定
- 特点: 原地排序，性能稳定

## 📚 3. 非比较排序算法

### 3.1 计数排序 (Counting Sort)

**算法思想**: 统计每个元素出现的次数，然后按顺序输出。

```cpp
template<typename T>
class CountingSort {
public:
    // 基础计数排序（适用于非负整数）
    static void countingSort(std::vector<int>& arr) {
        if (arr.empty()) return;
        
        int maxVal = *std::max_element(arr.begin(), arr.end());
        int minVal = *std::min_element(arr.begin(), arr.end());
        int range = maxVal - minVal + 1;
        
        std::vector<int> count(range, 0);
        std::vector<int> output(arr.size());
        
        // 统计每个元素的出现次数
        for (int num : arr) {
            count[num - minVal]++;
        }
        
        // 计算累积计数
        for (int i = 1; i < range; i++) {
            count[i] += count[i - 1];
        }
        
        // 构建输出数组（从后往前保证稳定性）
        for (int i = arr.size() - 1; i >= 0; i--) {
            output[count[arr[i] - minVal] - 1] = arr[i];
            count[arr[i] - minVal]--;
        }
        
        // 复制回原数组
        arr = output;
    }
    
    // 计数排序（字符串版本）
    static void countingSortForStrings(std::vector<std::string>& arr, int exp) {
        const int RADIX = 256; // ASCII字符数
        std::vector<int> count(RADIX, 0);
        std::vector<std::string> output(arr.size());
        
        // 统计字符出现次数
        for (const auto& str : arr) {
            int index = (exp < str.length()) ? str[exp] : 0;
            count[index]++;
        }
        
        // 计算累积计数
        for (int i = 1; i < RADIX; i++) {
            count[i] += count[i - 1];
        }
        
        // 构建输出数组
        for (int i = arr.size() - 1; i >= 0; i--) {
            int index = (exp < arr[i].length()) ? arr[i][exp] : 0;
            output[count[index] - 1] = arr[i];
            count[index]--;
        }
        
        arr = output;
    }
    
    // 带键值对的计数排序
    template<typename K, typename V>
    static void countingSortKeyValue(std::vector<std::pair<K, V>>& arr,
                                   std::function<int(const K&)> keyExtractor) {
        if (arr.empty()) return;
        
        int maxKey = keyExtractor(arr[0].first);
        int minKey = keyExtractor(arr[0].first);
        
        for (const auto& pair : arr) {
            int key = keyExtractor(pair.first);
            maxKey = std::max(maxKey, key);
            minKey = std::min(minKey, key);
        }
        
        int range = maxKey - minKey + 1;
        std::vector<int> count(range, 0);
        std::vector<std::pair<K, V>> output(arr.size());
        
        // 统计
        for (const auto& pair : arr) {
            count[keyExtractor(pair.first) - minKey]++;
        }
        
        // 累积
        for (int i = 1; i < range; i++) {
            count[i] += count[i - 1];
        }
        
        // 输出
        for (int i = arr.size() - 1; i >= 0; i--) {
            int key = keyExtractor(arr[i].first) - minKey;
            output[count[key] - 1] = arr[i];
            count[key]--;
        }
        
        arr = output;
    }
};
```

### 3.2 基数排序 (Radix Sort)

**算法思想**: 按照数字的各个位数进行多次计数排序。

```cpp
template<typename T>
class RadixSort {
public:
    // 基数排序（十进制）
    static void radixSort(std::vector<int>& arr) {
        if (arr.empty()) return;
        
        int maxVal = *std::max_element(arr.begin(), arr.end());
        
        // 对每个位数进行计数排序
        for (int exp = 1; maxVal / exp > 0; exp *= 10) {
            countingSortByDigit(arr, exp);
        }
    }
    
    // LSD基数排序（最低位优先）
    static void lsdRadixSort(std::vector<std::string>& arr) {
        if (arr.empty()) return;
        
        int maxLength = 0;
        for (const auto& str : arr) {
            maxLength = std::max(maxLength, static_cast<int>(str.length()));
        }
        
        // 从最低位开始排序
        for (int i = maxLength - 1; i >= 0; i--) {
            CountingSort<std::string>::countingSortForStrings(arr, i);
        }
    }
    
    // MSD基数排序（最高位优先）
    static void msdRadixSort(std::vector<std::string>& arr, int start, int end, int digit) {
        if (start >= end || digit < 0) return;
        
        const int RADIX = 256;
        std::vector<int> count(RADIX + 1, 0);
        
        // 统计字符频率
        for (int i = start; i <= end; i++) {
            int ch = (digit < arr[i].length()) ? arr[i][digit] : 0;
            count[ch + 1]++;
        }
        
        // 计算起始位置
        for (int i = 0; i < RADIX; i++) {
            count[i + 1] += count[i];
        }
        
        // 分类
        std::vector<std::string> temp(end - start + 1);
        for (int i = start; i <= end; i++) {
            int ch = (digit < arr[i].length()) ? arr[i][digit] : 0;
            temp[count[ch]++] = arr[i];
        }
        
        // 复制回原数组
        for (int i = 0; i < temp.size(); i++) {
            arr[start + i] = temp[i];
        }
        
        // 递归排序子数组
        for (int i = 0; i < RADIX; i++) {
            int subStart = start + count[i];
            int subEnd = start + count[i + 1] - 1;
            if (subStart < subEnd) {
                msdRadixSort(arr, subStart, subEnd, digit - 1);
            }
        }
    }
    
    // 二进制基数排序
    static void binaryRadixSort(std::vector<unsigned int>& arr) {
        if (arr.empty()) return;
        
        const int BITS = sizeof(unsigned int) * 8;
        std::vector<unsigned int> temp(arr.size());
        
        for (int bit = 0; bit < BITS; bit++) {
            int count0 = 0, count1 = 0;
            
            // 统计0和1的个数
            for (unsigned int num : arr) {
                if ((num >> bit) & 1) {
                    count1++;
                } else {
                    count0++;
                }
            }
            
            // 分配
            int index0 = 0, index1 = count0;
            for (unsigned int num : arr) {
                if ((num >> bit) & 1) {
                    temp[index1++] = num;
                } else {
                    temp[index0++] = num;
                }
            }
            
            arr = temp;
        }
    }
    
private:
    static void countingSortByDigit(std::vector<int>& arr, int exp) {
        const int RADIX = 10;
        std::vector<int> count(RADIX, 0);
        std::vector<int> output(arr.size());
        
        // 统计数字出现次数
        for (int num : arr) {
            count[(num / exp) % RADIX]++;
        }
        
        // 计算累积计数
        for (int i = 1; i < RADIX; i++) {
            count[i] += count[i - 1];
        }
        
        // 构建输出数组
        for (int i = arr.size() - 1; i >= 0; i--) {
            int digit = (arr[i] / exp) % RADIX;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }
        
        arr = output;
    }
};
```

### 3.3 桶排序 (Bucket Sort)

**算法思想**: 将元素分配到有限数量的桶中，然后对每个桶单独排序。

```cpp
template<typename T>
class BucketSort {
public:
    // 基础桶排序（浮点数）
    static void bucketSort(std::vector<double>& arr, int bucketCount = 10) {
        if (arr.empty()) return;
        
        // 创建桶
        std::vector<std::vector<double>> buckets(bucketCount);
        
        // 将元素分配到桶中
        for (double num : arr) {
            int bucketIndex = static_cast<int>(num * bucketCount);
            bucketIndex = std::min(bucketIndex, bucketCount - 1);
            buckets[bucketIndex].push_back(num);
        }
        
        // 对每个桶进行排序
        for (auto& bucket : buckets) {
            std::sort(bucket.begin(), bucket.end());
        }
        
        // 合并桶
        int index = 0;
        for (const auto& bucket : buckets) {
            for (double num : bucket) {
                arr[index++] = num;
            }
        }
    }
    
    // 整数桶排序
    static void bucketSortInt(std::vector<int>& arr, int bucketCount = 10) {
        if (arr.empty()) return;
        
        int minVal = *std::min_element(arr.begin(), arr.end());
        int maxVal = *std::max_element(arr.begin(), arr.end());
        int range = maxVal - minVal + 1;
        int bucketSize = (range + bucketCount - 1) / bucketCount;
        
        std::vector<std::vector<int>> buckets(bucketCount);
        
        // 分配到桶中
        for (int num : arr) {
            int bucketIndex = (num - minVal) / bucketSize;
            bucketIndex = std::min(bucketIndex, bucketCount - 1);
            buckets[bucketIndex].push_back(num);
        }
        
        // 对每个桶排序
        for (auto& bucket : buckets) {
            std::sort(bucket.begin(), bucket.end());
        }
        
        // 合并
        int index = 0;
        for (const auto& bucket : buckets) {
            for (int num : bucket) {
                arr[index++] = num;
            }
        }
    }
    
    // 字符串桶排序
    static void bucketSortString(std::vector<std::string>& arr, int position = 0) {
        if (arr.empty()) return;
        
        const int BUCKET_COUNT = 256; // ASCII字符数
        std::vector<std::vector<std::string>> buckets(BUCKET_COUNT);
        
        // 分配到桶中
        for (const auto& str : arr) {
            int bucketIndex = (position < str.length()) ? str[position] : 0;
            buckets[bucketIndex].push_back(str);
        }
        
        // 对每个桶递归排序（如果需要）
        int maxLength = 0;
        for (const auto& str : arr) {
            maxLength = std::max(maxLength, static_cast<int>(str.length()));
        }
        
        if (position < maxLength - 1) {
            for (auto& bucket : buckets) {
                if (bucket.size() > 1) {
                    bucketSortString(bucket, position + 1);
                }
            }
        }
        
        // 合并
        int index = 0;
        for (const auto& bucket : buckets) {
            for (const auto& str : bucket) {
                arr[index++] = str;
            }
        }
    }
    
    // 自适应桶排序
    static void adaptiveBucketSort(std::vector<double>& arr) {
        if (arr.empty()) return;
        
        // 根据数据分布决定桶数量
        double minVal = *std::min_element(arr.begin(), arr.end());
        double maxVal = *std::max_element(arr.begin(), arr.end());
        double range = maxVal - minVal;
        
        int bucketCount = std::max(1, static_cast<int>(std::sqrt(arr.size())));
        double bucketSize = range / bucketCount;
        
        std::vector<std::vector<double>> buckets(bucketCount);
        
        // 分配
        for (double num : arr) {
            int bucketIndex = static_cast<int>((num - minVal) / bucketSize);
            bucketIndex = std::min(bucketIndex, bucketCount - 1);
            buckets[bucketIndex].push_back(num);
        }
        
        // 排序和合并
        int index = 0;
        for (auto& bucket : buckets) {
            std::sort(bucket.begin(), bucket.end());
            for (double num : bucket) {
                arr[index++] = num;
            }
        }
    }
};
```

## 📊 排序算法性能对比

### 时间复杂度对比表

| 算法 | 最好时间 | 平均时间 | 最坏时间 | 空间复杂度 | 稳定性 | 原地排序 |
|------|----------|----------|----------|------------|--------|----------|
| 冒泡排序 | O(n) | O(n²) | O(n²) | O(1) | 稳定 | 是 |
| 选择排序 | O(n²) | O(n²) | O(n²) | O(1) | 不稳定 | 是 |
| 插入排序 | O(n) | O(n²) | O(n²) | O(1) | 稳定 | 是 |
| 希尔排序 | O(n log n) | O(n^1.3) | O(n²) | O(1) | 不稳定 | 是 |
| 归并排序 | O(n log n) | O(n log n) | O(n log n) | O(n) | 稳定 | 否 |
| 快速排序 | O(n log n) | O(n log n) | O(n²) | O(log n) | 不稳定 | 是 |
| 堆排序 | O(n log n) | O(n log n) | O(n log n) | O(1) | 不稳定 | 是 |
| 计数排序 | O(n+k) | O(n+k) | O(n+k) | O(k) | 稳定 | 否 |
| 基数排序 | O(d(n+k)) | O(d(n+k)) | O(d(n+k)) | O(n+k) | 稳定 | 否 |
| 桶排序 | O(n+k) | O(n+k) | O(n²) | O(n+k) | 稳定 | 否 |

### 选择指南

1. **小数据集 (n < 50)**:
   - 插入排序：实现简单，常数小
   - 选择排序：交换次数最少

2. **大数据集**:
   - 快速排序：平均性能最佳
   - 归并排序：稳定，最坏情况性能好
   - 堆排序：最坏情况性能保证

3. **特殊需求**:
   - 稳定排序：归并排序、计数排序
   - 原地排序：快速排序、堆排序
   - 整数排序：基数排序、计数排序

排序算法是算法学习的基础，掌握各种排序算法的实现和特性对于深入理解算法设计具有重要意义！🎯
