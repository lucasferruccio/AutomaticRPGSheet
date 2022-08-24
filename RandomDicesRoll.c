#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int result_dice[50], mod = 0, mod_place, adv_dis = 0, advdis_place;

int checkdice(char dice[]);

void roll_dice(int dice, int dice_quantity);

void split_dice(char dice[]);

int dice_quantity(char dice[], int split);

int dice_value(char dice[], int split);

int mod_calc(char dice[]);

void result(int modifier, int dice_quantity, int value);

void calc_advdis(char dice[]);

int main(int argc, char **argv)
{
    // Checa se só tem dois argumentos
    if (argc != 2)
    {
        printf("Sintaxe do dado errada\n");
        return 1;
    }
    int check = checkdice(argv[1]);
    if (check == 0)
    {
        split_dice(argv[1]);
        return 0;
    }
    return check;
}

int checkdice(char dice[])
{
    int aux1, aux2 = 0, aux3, aux4 = 0, lim2 = strlen(dice);
    for (int i = 0, lim = strlen(dice); i < lim; i++)
    {
        // Checa se a quantidade de dados são compostos somente de números
        if (dice[i] == 'd' && i != 0)
        {
            aux1 = i + 1;
            aux3 = i;
            for (int j = 0; j < i; j++)
            {
                if (!isdigit(dice[j]))
                {
                    printf("Erro: 1 (quantidade de dados)\n");
                    return 1;
                }
            }
        }
        // Checa se tem vantagem ou desvantagem para poder checar o valor do dado
        else if (dice[i] == 'k')
        {
            if (dice[i + 1] == 'l' || dice[i + 1] == 'h')
            {
                aux2 = 1;
                for (int n = aux1; n < i; n++)
                {
                    if (!isdigit(dice[n]))
                    {
                        printf("Erro: 2 (Valor do dado)\n");
                        return 2;
                    }
                }
            }
            else
            {
                printf("Erro: 4 (Vant|desv ?)\n");
                return 4;
            }
        }
        // Checa se tem algum modificador e se é válido, caso tenha salva um novo limite par apoder validar o valor do dado
        else if (dice[i] == '+' || dice[i] == '-')
        {
            lim2 = i;
            for (int m = i + 1; m < lim; m++)
            {
                if (!isdigit(dice[m]))
                {
                    printf("Erro: 3 (Valor do modificador)\n");
                    return 3;
                }
            }
        }
    }
    // Caso não tenha vantagem aqui será checado o valor do dado
    if (aux2 == 0)
    {
        for (int o = aux3 + 1; o < lim2; o++)
        {
            if (!isdigit(dice[o]))
            {
                printf("Erro: 2 (Valor do dado)\n");
                return 2;
            }
        }
    }
    return 0;
}

void split_dice(char dice[])
{
    int lengh_dice = strlen(dice), split, value, quantity, mod_value;
    // Separa os valores da quantidade de dados, o valor do dado e o valor do modificador e marca um aviso caso tenha vant/desv
    for (int i = 0; i <= lengh_dice; i++)
    {
        if (dice[i] == 'd')
        {
            split = i;
        }
        if (dice[i] == '+' || dice[i] == '-')
        {
            mod = 1;
            mod_place = i;
            mod_value = mod_calc(dice);
        }
        if (dice[i] == 'k')
        {
            advdis_place = i;
        }
    }
    calc_advdis(dice);
    quantity = dice_quantity(dice, split);
    value = dice_value(dice, split);
    roll_dice(value, quantity);
    result(mod_value, quantity, value);
    return;
}

void calc_advdis(char dice[])
{
    // Marca se tem vantagem - 1, desvantagem - 2
    if (dice[advdis_place + 1] == 'h')
    {
        adv_dis = 1;
    }
    else if (dice[advdis_place + 1] == 'l')
    {
        adv_dis = 2;
    }
    return;
}

int dice_quantity(char dice[], int split)
{
    // Separa e retorna a quantidade de dados, caso não seja digitado um valor sendo deixado em branco ele assume que é um único dado
    char quantity[10];
    if (split == 0)
    {
        return 1;
    }
    else
    {
        for (int i = 0; i < split; i++)
        {
            quantity[i] = dice[i];
        }
        return atoi(quantity);
    }
}

int dice_value(char dice[], int split)
{
    char value[10];
    int j = 0, end, start = split + 1, especial;
    // Calcula o final do dado dependendo das condições (Vant\Desv e mod)
    if (adv_dis != 0)
    {
        end = advdis_place;
    }
    else if (mod == 1)
    {
        end = mod_place;
    }
    else
    {
        end = strlen(dice);
    }
    // Separa e retorna o valor do dado
    especial = end - start;
    if (especial == '1')
    {
        return dice[start + 1];
    }
    for (int i = start; i < end; i++)
    {
        value[j] = dice[i];
        j++;
    }
    return atoi(value);
}

void roll_dice(int dice, int dice_quantity)
{
    // Roda os dados
    int aux2 = mod;
    srand(time(NULL));
    printf("Valores dos Dados:");
    int j = 0;
    for (int i = 1; i <= dice_quantity; i++)
    {
        int aux = rand() % dice;
        result_dice[j] = aux + 1;
        printf(" {%i}", result_dice[j]);
        j++;
    }
    mod = aux2;
    printf("\n");
    return;
}

int mod_calc(char dice[])
{
    // Calcula o modificador dos dados
    char mod_aux[10];
    int end = strlen(dice), j = 0;
    for (int i = mod_place; i < end; i++)
    {
        mod_aux[j] = dice[i];
        j++;
    }

    return atoi(mod_aux);
}

void result(int modifier, int dice_quantity, int value)
{
    int final;
    // Pega o maior caso seja com vantagem
    if (adv_dis == 1)
    {
        int high = 0;
        for (int i = 0; i < dice_quantity; i++)
        {
            if (result_dice[i] > high)
            {
                high = result_dice[i];
            }
        }
        if (mod == 1)
        {
            high += modifier;
        }
        final = high;
    }
    // Pega o menor caso tenha desvantagem
    else if (adv_dis == 2)
    {
        int low = value;
        for (int i = 0; i < dice_quantity; i++)
        {
            if (result_dice[i] < low)
            {
                low = result_dice[i];
            }
        }
        if (mod == 1)
        {
            low += modifier;
        }
        final = low;
    }
    // Soma os valores dos dados caso não haja vantagens e soma os modificador caso exista
    else
    {
        int soma = 0;
        for (int i = 0; i < dice_quantity; i++)
        {
            soma += result_dice[i];
        }
        if (mod == 1)
        {
            soma += modifier;
        }
        final = soma;
    }

    printf("Resultado: %i\n", final);
    return;
}
