//
// Created by KWAZAR_ on 03.04.2024.
//

#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int code = INCORRECT_MATRIX;

  if (rows >= 1 && columns >= 1 && result != NULL) {
    result->matrix = calloc(rows, sizeof(double *));
    if (result->matrix != NULL) {
      code = OK;

      for (int i = 0; i < rows; i++) {
        result->matrix[i] = calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
          s21_remove_matrix(result);
          code = ALLOCATION_ERROR;
          break;
        }
      }
    } else
      code = ALLOCATION_ERROR;
  }

  if (code == OK) {
    result->rows = rows;
    result->columns = columns;
  }

  return code;
}

void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);

  A->matrix = NULL;
  A->rows = 0;
  A->columns = 0;
}

int is_correct(matrix_t *A) {
  int code = SUCCESS;

  if (!A || (A->rows < 1) || (A->columns < 1)) code = FAILURE;

  return code;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int code = SUCCESS;

  if (!A || !B || (A->columns != B->columns) || (A->rows != B->rows)) {
    code = FAILURE;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
          code = FAILURE;
          break;
        }
      }
    }
  }

  return code;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code = OK;

  if (!is_correct(A) || !is_correct(B))
    code = INCORRECT_MATRIX;
  else if (A->rows != B->rows || A->columns != B->columns)
    code = CALCULATION_ERROR;
  else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }

  return code;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code = OK;

  if (!is_correct(A) || !is_correct(B))
    code = INCORRECT_MATRIX;
  else if (A->rows != B->rows || A->columns != B->columns)
    code = CALCULATION_ERROR;
  else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }

  return code;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int code = OK;

  if (!is_correct(A))
    code = INCORRECT_MATRIX;
  else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }

  return code;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code = OK;

  if (!is_correct(A))
    code = INCORRECT_MATRIX;
  else if (A->columns != B->rows)
    code = CALCULATION_ERROR;
  else {
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int k = 0; k < B->rows; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }

  return code;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int code = OK;

  if (!is_correct(A))
    code = INCORRECT_MATRIX;
  else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; ++j) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }

  return code;
}

void s21_minor(matrix_t *A, matrix_t *minor, int row, int col) {
  int minorRow = 0, minorCol = 0;

  for (int i = 0; i < A->rows; i++) {
    if (i == row) continue;

    for (int j = 0; j < A->columns; j++) {
      if (j == col) continue;

      minor->matrix[minorRow][minorCol++] = A->matrix[i][j];

      if (minorCol == A->columns - 1) {
        minorRow++;
        minorCol = 0;
      }
    }
  }
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int code = OK;

  if (!is_correct(A))
    code = INCORRECT_MATRIX;

  else if (A->rows != A->columns)
    code = CALCULATION_ERROR;

  else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        matrix_t minor;
        s21_create_matrix(A->rows - 1, A->columns - 1, &minor);

        if (minor.matrix == NULL) {
          s21_remove_matrix(&minor);
          code = CALCULATION_ERROR;
          break;
        } else {
          s21_minor(A, &minor, i, j);
          double temp_result;
          code = s21_determinant(&minor, &temp_result);
          if (code != OK) {
            s21_remove_matrix(&minor);
            break;
          } else {
            int sign = ((i + j) % 2 == 0) ? 1 : -1;
            result->matrix[i][j] = sign * temp_result;
          }
        }
        s21_remove_matrix(&minor);
      }
    }
  }

  return code;
}

int s21_determinant(matrix_t *A, double *result) {
  int code = OK;

  if (!is_correct(A))
    code = INCORRECT_MATRIX;

  else if (A->rows != A->columns)
    code = CALCULATION_ERROR;

  else if (A->rows == 1)
    *result = A->matrix[0][0];

  else {
    double determinant = 0;
    matrix_t minor;
    s21_create_matrix(A->rows - 1, A->columns - 1, &minor);

    if (minor.matrix != NULL) {
      double temp;

      for (int i = 0; i < A->columns; i++) {
        int sign = (i % 2 == 0) ? 1 : -1;
        s21_minor(A, &minor, 0, i);
        code = s21_determinant(&minor, &temp);

        if (code != OK) break;

        determinant += sign * A->matrix[0][i] * temp;
      }

      *result = determinant;
    } else
      code = CALCULATION_ERROR;

    s21_remove_matrix(&minor);
  }

  return code;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int code = OK;
  double det = 0;
  s21_determinant(A, &det);

  if (!is_correct(A))
    code = INCORRECT_MATRIX;

  else if (A->rows != A->columns || det == 0)
    code = CALCULATION_ERROR;

  else {
    matrix_t temp_result, r2;
    // s21_create_matrix(A->rows, A->columns, &temp_result);
    s21_create_matrix(A->rows, A->columns, &r2);
    code = s21_calc_complements(A, &temp_result);
    if (code == OK) {
      code = s21_transpose(&temp_result, &r2);
      if (code == OK) {
        double inv_det = 1.0 / det;
        code = s21_mult_number(&r2, inv_det, result);
      } else
        code = CALCULATION_ERROR;
    } else
      code = CALCULATION_ERROR;
    s21_remove_matrix(&r2);
    //  s21_remove_matrix(&temp_result);
  }

  return code;
}
