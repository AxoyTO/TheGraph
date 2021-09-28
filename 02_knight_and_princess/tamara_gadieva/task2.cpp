#include "task2.hpp"

int main() {
  FILE* fd;
  int* edge_ids;

  //вспомогательная матрица нумерации рёбер
  if (!(fd = fopen("graph.json", "w"))) {
    std::cerr << "Can't open/create file";
    return 1;
  }
  edge_ids = edge_ids_init();
  vertices_json_output(fd, edge_ids);
  edges_json_output(fd, edge_ids);
  edge_ids_free(edge_ids);
  fclose(fd);
  return 0;
}

//создаём динамический массив и заполняем данными о номерах
//ребёр, соединяющих вершины графа
int* edge_ids_init(void) {
  int* edge_ids = (int*)malloc(sizeof(int) * VERTEX_NUM * VERTEX_NUM);

  int edge_count = 0;
  //счётчик для нумерации вершин

  for (int i = 0; i < VERTEX_NUM; i++)
    for (int j = 0; j < VERTEX_NUM; j++)
      edge_ids[i * VERTEX_NUM + j] = -1;

  for (int i = 0; i < VERTEX_NUM; i++) {
    for (int j = i; j < VERTEX_NUM; j++) {
      if (adj_matrix[i][j] == 1) {
        edge_ids[i * VERTEX_NUM + j] = edge_count;
        edge_count++;
      }
    }
  }
  //нумеруем рёбра, проходя по элементам верхнего
  //треугольника матрицы связности графа

  return edge_ids;
}

//чистка динамической памяти вспомогательной матрицы
void edge_ids_free(int* edge_ids) {
  free(edge_ids);
}

//вывод информации о вершинах графа в файл
void vertices_json_output(FILE* fd, int* edge_ids) {
  int first_num_flag;
  //флаг первого числа в списке ребёр для выставления запятых

  char help_str[HELP_STR_LEN];
  //вспомогательная строка для перевода чисел в строки

  fprintf(fd, "{\n");
  fprintf(fd, "\t\"vertices\": [\n");

  for (int i = 0; i < VERTEX_NUM; i++) {
    fprintf(fd, "\t\t{ \"id\": ");
    sprintf(help_str, "%d", i);
    fprintf(fd, "%s", help_str);
    fprintf(fd, ", \"edge_ids\": [");
    first_num_flag = 1;

    for (int j = 0; j < VERTEX_NUM; j++) {
      if ((adj_matrix[i][j] == 1) && (i < j)) {
        if (!first_num_flag)
          fprintf(fd, ", ");
        else
          first_num_flag = 0;
        sprintf(help_str, "%d", edge_ids[i * VERTEX_NUM + j]);
        fprintf(fd, "%s", help_str);
      }
      if ((adj_matrix[i][j] == 1) && (i > j)) {
        if (!first_num_flag)
          fprintf(fd, ", ");
        else
          first_num_flag = 0;
        sprintf(help_str, "%d", edge_ids[j * VERTEX_NUM + i]);
        fprintf(fd, "%s", help_str);
      }
    }

    if (i != (VERTEX_NUM - 1))
      fprintf(fd, "] },\n");
    else
      fprintf(fd, "] }\n");
  }
  fprintf(fd, "\t],\n");
}

void edges_json_output(FILE* fd, int* edge_ids) {
  int first_str_flag = 1;
  //флаг используется для обозначения первой строки вывода

  char help_str[HELP_STR_LEN];
  //вспомогательная строка для перевода чисел в строки

  fprintf(fd, "\t\"edges\": [");
  for (int i = 0; i < VERTEX_NUM; i++) {
    for (int j = 1; j < VERTEX_NUM; j++) {
      if (edge_ids[i * VERTEX_NUM + j] != (-1)) {
        if (first_str_flag) {
          fprintf(fd, "\n");
          first_str_flag = 0;
        } else
          fprintf(fd, ",\n");
        fprintf(fd, "\t\t{ \"id\": ");
        sprintf(help_str, "%d", edge_ids[i * VERTEX_NUM + j]);
        fprintf(fd, "%s, \"vertex_ids\": [", help_str);
        sprintf(help_str, "%d", i);
        fprintf(fd, "%s, ", help_str);
        sprintf(help_str, "%d", j);
        fprintf(fd, "%s] }", help_str);
      }
    }
  }
  fprintf(fd, "\n\t]\n}");
}
