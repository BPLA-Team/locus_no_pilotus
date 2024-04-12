#pragma once

#include <limits>
#include <vector>

namespace math {

#define inf std::numeric_limits<double>::infinity()

/// @brief структура для хранения двух минимумов строки/столбца
struct Minimums {
  double first;
  double second;
};

/// @brief матрица смежности для алгоритма Литтла
class AdjacencyMatrix {
 public:
  /**
   * @brief создает новый экземпляр AdjacencyMatrix с
   * добавлением строки и столбца номеров точек
   * @param nums: матрица смежности графа
   * @return экземпляр AdjacencyMatrix по данной матрице смежности
   */
  static AdjacencyMatrix WithExtraRowCol(std::vector<std::vector<double>> nums);

  enum Mins { Rows, Columns };

  // Копирующее присваивание для матрицы
  AdjacencyMatrix& operator=(const AdjacencyMatrix& m);

  /**
   * @brief изменение элемента матрицы
   * @param i: номер строки элемента
   * @param j: номер столбца элемента
   * @param num: новое значение элемента
   */
  void SetMatrixValue(std::size_t i, std::size_t j, double num);

  // Возвращает размер матрицы
  std::size_t GetSize() const { return size_; }

  // Возвращает элемент матрицы
  double GetMatrixValue(std::size_t i, std::size_t j) const {
    return matrix_[i][j];
  }

  // Возвращает оценку расстояния
  double GetBottomLineEvaluation() const { return evaluation_; }

  // Возвращает выбранное на данной итерации
  // ребро для последующего рассмотрения
  std::pair<std::size_t, std::size_t> GetSelectedEdge() const {
    return selected_edge_;
  }

  // Возвращает выбранное на данной итерации
  // значение матрицы для последующего рассмотрения
  std::pair<std::size_t, std::size_t> GetSelectedValue() const {
    return selected_value_;
  }

  /**
   * @brief создает минор матрицы
   * @param i: номер удаляемой строки
   * @param j: номер удаляемого столбца
   * @return AdjacencyMatrix минор матрицы
   */
  AdjacencyMatrix Minor(std::size_t i, std::size_t j);

  /**
   * @brief создает редуцированную матрицы
   * @return AdjacencyMatrix редуцированная матрица
   */
  AdjacencyMatrix Reducted();

  // Считает данные для матрицы
  void CalculateData();

  /**
   * @brief расширяет матрицу для нескольких коммивояжеров
   * @param num_of_flyers: количество коммивояжеров
   */
  void ExtendTo(std::size_t num_of_flyers);

 private:
  // Размер матрицы
  std::size_t size_;

  // Матрица
  std::vector<std::vector<double>> matrix_;

  // Редуцированная версия матрицы
  std::vector<std::vector<double>> reducted_matrix_;

  // Минимальный элемент в каждой строке и в каждом столбце
  std::vector<double> min_numbers_;

  // Оценка пути для данной матрицы
  double evaluation_ = 0;

  // Ребро, которое выбирается для следующего шага в алгоритме Литтла
  std::pair<std::size_t, std::size_t> selected_edge_;

  // Значение матрицы, которое выбирается для следующего шага в алгоритме
  // Литтла
  std::pair<std::size_t, std::size_t> selected_value_;

  /**
   * @brief создает новый экземпляр AdjacencyMatrix
   * @param nums: матрица смежности графа
   * @return экземпляр AdjacencyMatrix по данной матрице смежности
   */
  AdjacencyMatrix(std::vector<std::vector<double>> nums);

  /**
   * @brief находит 2 минимума в строке или столбце
   * @param type: тип поиска минимума(в строке или в столбце)
   * @param index: номер строки/столбца
   * @return Minimums 2 найденных минимума
   */
  Minimums FindTwoMinimums(Mins type, std::size_t index) const;

  /**
   * @brief находит нижнюю оценку для матрицы
   * @return double нижняя оценку матрицы
   */
  double BottomLineEvaluation();

  /**
   * @brief находит позицию нуля с наибольшей степенью
   * @return позицию нуля с наибольшей степенью
   */
  std::pair<std::size_t, std::size_t> HighestPowerOfZero() const;
};

}  // namespace math
