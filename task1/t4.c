#include <stdio.h>

#define N 5

void init_matrix(int matrix[N][N])
{
    int pos_upper_horizontal = 0;
    int pos_lower_horizontal = N;
    int pos_left_vertical = 0;
    int pos_right_vertical = N;
    int counter = 1;
    int end_i = N/2;
    int end_j = N%2 == 0 ? N/2-1 : N/2;
    int i = 0;
    int j = 0;
    while(1)
    {
        // Обход до Правой Вертикальной границы
        for(; j < pos_right_vertical; j++)
            matrix[i][j] = counter++;
        j--; // Уменьшаем счетчик, чтобы не выпасть за границы матрицы
        // Обход до Нижней Горизонтальной границы
        for(i++; i < pos_lower_horizontal; i++)
            matrix[i][j] = counter++;
        i--;
        for(j--; j >= pos_left_vertical; j--)
            matrix[i][j] = counter++;
        j++; // здесь j единицу меньше из-за условия for >=, поэтому делаем +1
        if(i == end_i && j == end_j)  
            break;

        for(i--; i > pos_upper_horizontal; i--)
            matrix[i][j] = counter++;
        
        i++;
        j++;
        pos_upper_horizontal++;
        pos_lower_horizontal--;
        pos_left_vertical++;
        pos_right_vertical--;
    }

    
}

void print_matrix(int matrix[N][N]) {
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

int main()
{
    int matrix[N][N];
    init_matrix(matrix);
    print_matrix(matrix);
}