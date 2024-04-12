/*======================================================================================================================*/
/*ICSF13 - 2023-2 - TRABALHO 2                                                                                          */
/*======================================================================================================================*/
/*Alexandre Alberto Menon. RA: 2603403                                                                                  */
/*Giovanni Mioto. RA: 2603454                                                                                           */
/*Jennyfer Mikaelly Melo da Silva. RA: 2603470                                                                          */
/*======================================================================================================================*/
#include "trabalho3.h"
#include "imagem.h"
#include <limits.h>
#include <math.h>
/*======================================================================================================================*/
//Declara��o das fun��es usadas no c�digo
void imagem_binaria(Imagem1C* img);
void trata_circulo(Imagem1C* img);
void rotulo_imagem(Imagem1C* img, int* rotulo, int* menor_rotulo);
void identifica_bolinha(Imagem1C* img, int* rotulo_atual, int* rotulo, int* primeiro_maior, int* valor_primeiro_maior, int* segundo_maior, int* valor_segundo_maior);
void encontra_y_centro(Imagem1C* img, int* valor_primeiro_maior, int* valor_segundo_maior, int* flag_cyb1, int* flag_cyb2, int* comecoy_bola_1, int* comecoy_bola_2, int* finaly_bola_1, int* finaly_bola_2, int* y1, int* y2);
void encontra_x_centro(Imagem1C* img, int* valor_primeiro_maior, int* valor_segundo_maior, int* flag_cyb1, int* flag_cyb2, int* comecox_bola_1, int* comecox_bola_2, int* finalx_bola_1, int* finalx_bola_2, int* x1, int* x2);
/*======================================================================================================================*/

double detectaSensorBar (Imagem1C* img, Coordenada* l, Coordenada* r)
{
/*======================================================================================================================*/
//Declara��o de vari�veis
    int rotulo=1, menor_rotulo, rotulo_atual=1;
    int primeiro_maior=0, segundo_maior=0, valor_primeiro_maior, valor_segundo_maior;
    int comecoy_bola_1, comecoy_bola_2, comecox_bola_1, comecox_bola_2;
    int finaly_bola_1, finaly_bola_2, finalx_bola_1, finalx_bola_2;
    int flag_cyb1=1, flag_cyb2=1;
    double tangente;
    int y1, y2, x1, x2;
 /*======================================================================================================================*/
//Chamada das fun��es
    imagem_binaria(img);
    trata_circulo(img);
    rotulo_imagem(img, &rotulo, &menor_rotulo);
    identifica_bolinha(img, &rotulo_atual, &rotulo, &primeiro_maior, &valor_primeiro_maior, &segundo_maior, &valor_segundo_maior);
    encontra_y_centro(img, &valor_primeiro_maior, &valor_segundo_maior, &flag_cyb1, &flag_cyb2, &comecoy_bola_1, &comecoy_bola_2, &finaly_bola_1, &finaly_bola_2, &y1, &y2);
    encontra_x_centro(img, &valor_primeiro_maior, &valor_segundo_maior, &flag_cyb1, &flag_cyb2, &comecox_bola_1, &comecox_bola_2, &finalx_bola_1, &finalx_bola_2, &x1, &x2);
/*======================================================================================================================*/
//Atribui��o das coordenadas �s suas respectivas posi��es
    if(x1<x2)
    {
        l->x=x1;
        l->y=y1;
        r->x=x2;
        r->y=y2;
    }
    else
    {
        l->x=x2;
        l->y=y2;
        r->x=x1;
        r->y=y1;
    }
/*======================================================================================================================*/
//C�lculo da tangente
    tangente=((double)((l->y)-(r->y))/((l->x)-(r->x)));
    tangente=atan(tangente);
    return tangente;
}

void imagem_binaria(Imagem1C* img)
//De modo que s� existam 2 poss�veis valores na imagem, � atribu�do 0 a valores menores que 127 e 255 caso contr�rio,al�m disso os pixels das bordas viram 0
{
    int i, j;

    for(i=0; i<img->altura; i++)
    {
        for(j=0; j<img->largura; j++)
        {
/*======================================================================================================================*/
//Caso esteja em uma das bordas da imagem, transforma a mesma em preto
            if(i==0)
            {
                img->dados[i][j]=0;
            }
            else if(i==img->altura-1)
            {
                img->dados[i][j]=0;
            }
            if(j==0)
            {
                img->dados[i][j]=0;


            }
            else if(j==img->largura-1)
            {
                img->dados[i][j]=0;
            }
/*======================================================================================================================*/
//Binariza��o da imagem
            if(img->dados[i][j]<127)
            {
                img->dados[i][j]=0;
            }
            else
            {
                img->dados[i][j]=255;
            }
        }
    }
/*======================================================================================================================*/
}

void trata_circulo(Imagem1C* img)
//Essa fun��o identifica se a maioria dos valores vizinhos de um pixel possuem o valor diferente dele e atribuem tal valor a esse pixel
{
    int i, j;

    for(i=1; i<img->altura-1; i++)
    {
        for(j=1; j<img->largura-1; j++)
        {
/*======================================================================================================================*/
//Verifica se existem vizinhos brancos ou pretos e atribui de acordo com a maioria ao redor
            if(img->dados[i][j]==0)
            {
                if(img->dados[i+1][j]==255 && img->dados[i-1][j]==255 && img->dados[i][j+1]==255 && img->dados[i][j-1]==255)//Verifica��o para pixels brancos vizinhos
                {
                    img->dados[i][j]=255;
                }
            }
            else
            {
                if(img->dados[i+1][j]==0 && img->dados[i-1][j]==0 && img->dados[i][j+1]==0 && img->dados[i][j-1]==0)//Verifica��o para pixels pretos vizinhos
                {
                    img->dados[i][j]=0;
                }
            }
        }
    }
/*======================================================================================================================*/
}

void rotulo_imagem(Imagem1C* img, int* rotulo, int* menor_rotulo)
/*Essa fun��o identifica se um pixel difere de seus arredores, e rotula o mesmo*/
{
    int i, j;
/*======================================================================================================================*/
//Adiciona o r�tulo de cima para baixo e da esquerda para a direita
    for(i=1; i<img->altura; i++)
    {
        for(j=1; j<img->largura-1; j++)
        {
            if(img->dados[i][j]==255)//Verifica se a posi��o � um pixel branco
            {
                if(img->dados[i][j-1]==0 && img->dados[i-1][j-1]==0 && img->dados[i-1][j]==0 && img->dados[i-1][j+1]==0)//Verifica se as posi��es ao redor ainda n�o possuem r�tulo
                {
                    img->dados[i][j]=*rotulo;//Caso n�o possua vizinhos que s�o r�tulos, essa posi��o recebe um r�tulo
                    (*rotulo)++;
                }
                else //Caso um vizinho possua um r�tulo
                {
                    *menor_rotulo=INT_MAX;//Atribui um n�mero inteiro m�ximo � vari�vel e verifica qual � o menor r�tulo ao seu redor
                    if(*menor_rotulo>img->dados[i][j-1] && img->dados[i][j-1]!=0)
                    {
                        *menor_rotulo=img->dados[i][j-1];
                    }
                    if(*menor_rotulo>img->dados[i-1][j-1] && img->dados[i-1][j-1]!=0)
                    {
                        *menor_rotulo=img->dados[i-1][j-1];
                    }
                    if(*menor_rotulo>img->dados[i-1][j] && img->dados[i-1][j]!=0)
                    {
                        *menor_rotulo=img->dados[i-1][j];
                    }
                    if(*menor_rotulo>img->dados[i-1][j+1] && img->dados[i-1][j+1]!=0)
                    {
                        *menor_rotulo=img->dados[i-1][j+1];
                    }
                    img->dados[i][j]=*menor_rotulo;//Atribui o menor r�tulo encontrado � posi��o em quest�o
                }
            }
        }
    }
/*======================================================================================================================*/
//Adiciona o r�tulo de baixo para cima e da direita para a esquerda
    for(i=img->altura-2; i>0; i--)
    {
        for(j=img->largura-2; j>0; j--)
        {
            if(img->dados[i][j]!=0)//Verifica se a posi��o � um pixel rotulado
            {
                *menor_rotulo=img->dados[i][j];//Atribui o r�tulo da posi��o como sendo o menor r�tulo e verifica se n�o existem r�tulos menores ao redor
                if(*menor_rotulo>img->dados[i][j+1] && img->dados[i][j+1]!=0)
                {
                    *menor_rotulo=img->dados[i][j+1];
                }
                if(*menor_rotulo>img->dados[i+1][j+1] && img->dados[i+1][j+1]!=0)
                {
                    *menor_rotulo=img->dados[i+1][j+1];
                }
                if(*menor_rotulo>img->dados[i+1][j] && img->dados[i+1][j]!=0)
                {
                    *menor_rotulo=img->dados[i+1][j];
                }
                if(*menor_rotulo>img->dados[i+1][j-1] && img->dados[i+1][j-1]!=0)
                {
                    *menor_rotulo=img->dados[i+1][j-1];
                }
                img->dados[i][j]=*menor_rotulo;//Atribui o menor r�tulo encontrado � posi��o analisada
            }
        }
    }
/*======================================================================================================================*/
}

void identifica_bolinha(Imagem1C* img, int* rotulo_atual, int* rotulo, int* primeiro_maior, int* valor_primeiro_maior, int* segundo_maior, int* valor_segundo_maior)
/*Essa fun��o identifica a posi��o das bolinhas levando em conta os dois maiores aglomerados de r�tulos presentes na imagem*/
{
    int i, j, cont=0;

/*======================================================================================================================*/
//Passa pela matriz da imagem at� o rotulo atual alcan�ar o r�tulo m�ximo atingido pela imagem
    while(*rotulo_atual<*rotulo)
    {
        for(i=1; i<img->altura-1; i++)
        {
            for(j=1; j<img->largura-1; j++)
            {
                if(img->dados[i][j]==*rotulo_atual)//Caso a posi��o tenha o r�tulo que est� sendo contado, adiciona ao contador
                {
                    cont++;
                }
            }
        }
        if(cont>*primeiro_maior)//verifica se o r�tulo atual � o maior, caso seja, o salva na vari�vel
        {
            *primeiro_maior=cont;
            *valor_primeiro_maior=*rotulo_atual;
        }
        (*rotulo_atual)++;//Vai para o pr�ximo r�tulo a ser analisado
        cont=0;//Zera o contador para come�ar a an�lise novamente
    }
/*======================================================================================================================*/
//Realiza o mesmo processo visto anteriormente, mas dessa vez para achar o segundo maior aglomerado de pixels rotulados
    *rotulo_atual=1;
    while(*rotulo_atual<*rotulo)
    {
        for(i=1; i<img->altura-1; i++)
        {
            for(j=1; j<img->largura-1; j++)
            {
                if(*rotulo_atual!=*valor_primeiro_maior && img->dados[i][j]==*rotulo_atual)//�nica diferen�a � que esse processo � feito excluindo as posi��es que possuem o r�tulo salvo anteriormente
                {
                    cont++;
                }
            }
        }
        if(cont>*segundo_maior)
        {
            *segundo_maior=cont;
            *valor_segundo_maior=*rotulo_atual;
        }
        (*rotulo_atual)++;
        cont=0;
    }
/*======================================================================================================================*/
}

void encontra_y_centro(Imagem1C* img, int* valor_primeiro_maior, int* valor_segundo_maior, int* flag_cyb1, int* flag_cyb2, int* comecoy_bola_1, int* comecoy_bola_2, int* finaly_bola_1, int* finaly_bola_2, int* y1, int* y2)
/*Essa fun��o encontra o come�o e o final de cada bolinha em termos de y, e encontra o y do centro por meio do pixel m�dio desse intervalo */
{
    int i, j;

    for(i=1; i<img->altura; i++)
    {
        for(j=1; j<img->largura; j++)
        {
/*======================================================================================================================*/
//Faz a passagem pela imagem at� achar os primeiros pixels onde os 2 maiores r�tulos aparecem
            if(img->dados[i][j]==*valor_primeiro_maior && *flag_cyb1)
            {
                *comecoy_bola_1=i;//Salva a posi��o do eixo y onde foi encontrado o primeiro pixel com o maior r�tulo
                *flag_cyb1=0;//cyb1=come�o, no eixo y, da bola 1
            }
            if(img->dados[i][j]==*valor_segundo_maior && *flag_cyb2)
            {
                *comecoy_bola_2=i;//Salva a posi��o do eixo y onde foi encontrado o primeiro pixel com o segundo maior r�tulo
                *flag_cyb2=0;//cyb2=come�o, no eixo y, da bola 2
            }
        }
    }
//Reinicia as flags
    *flag_cyb1=1;
    *flag_cyb2=1;
/*======================================================================================================================*/
//Realiza o mesmo processo de cima, mas desta vez de baixo para cima, para achar o final das bolinhas com rela��o ao eixo y
    for(i=img->altura-1; i>0; i--)
    {
        for(j=img->largura-1; j>0; j--)
        {
            if(img->dados[i][j]==*valor_primeiro_maior && *flag_cyb1)
            {
                *finaly_bola_1=i;
                *flag_cyb1=0;
            }
            if(img->dados[i][j]==*valor_segundo_maior && *flag_cyb2)
            {
                *finaly_bola_2=i;
                *flag_cyb2=0;
            }
        }
    }
/*======================================================================================================================*/
//C�lculo para achar a posi��o central das bolinhas com rela��o ao eixo y
    *y1=((*finaly_bola_1-*comecoy_bola_1)/2)+*comecoy_bola_1;
    *y2=((*finaly_bola_2-*comecoy_bola_2)/2)+*comecoy_bola_2;
//Reinicia as flag que ser�o chamadas para a fun��o encontra_x_centro
    *flag_cyb1=1;
    *flag_cyb2=1;
}

void encontra_x_centro(Imagem1C* img, int* valor_primeiro_maior, int* valor_segundo_maior, int* flag_cyb1, int* flag_cyb2, int* comecox_bola_1, int* comecox_bola_2, int* finalx_bola_1, int* finalx_bola_2, int* x1, int* x2)
/*Essa fun��o encontra o come�o e o final de cada bolinha em termos de x, e encontra o x do centro por meio do pixel m�dio desse intervalo */
{
    int i, j;
    for(i=1; i<img->largura-1; i++)
    {
        for(j=1; j<img->altura-1; j++)
        {
/*======================================================================================================================*/
//Faz a passagem pela imagem at� achar os primeiros pixels onde os 2 maiores r�tulos aparecem
            if(img->dados[j][i]==*valor_primeiro_maior && *flag_cyb1)
            {
                *comecox_bola_1=i;//Salva a posi��o do eixo x onde foi encontrado o primeiro pixel com o maior r�tulo
                *flag_cyb1=0;//1cyb=come�o, no eixo x, da bola 1
            }
            if(img->dados[j][i]==*valor_segundo_maior && *flag_cyb2)
            {
                *comecox_bola_2=i;//Salva a posi��o do eixo x onde foi encontrado o primeiro pixel com o segundo maior r�tulo
                *flag_cyb2=0;//cyb2=come�o, no eixo x, da bola 2
            }
        }
    }
//Reinicia as flags
    *flag_cyb1=1;
    *flag_cyb2=1;
/*======================================================================================================================*/
//Realiza o mesmo processo de cima, mas desta vez de baixo para cima, para achar o final das bolinhas com rela��o ao eixo x

    for(i=img->largura-1; i>0; i--)
    {
        for(j=img->altura-1; j>0; j--)
        {
            if(img->dados[j][i]==*valor_primeiro_maior && *flag_cyb1)
            {
                *finalx_bola_1=i;
                *flag_cyb1=0;
            }
            if(img->dados[j][i]==*valor_segundo_maior && *flag_cyb2)
            {
                *finalx_bola_2=i;
                *flag_cyb2=0;
            }
        }
    }
/*======================================================================================================================*/
//C�lculo para achar a posi��o central das bolinhas com rela��o ao eixo x
    *x1=((*finalx_bola_1-*comecox_bola_1)/2)+*comecox_bola_1;
    *x2=((*finalx_bola_2-*comecox_bola_2)/2)+*comecox_bola_2;
}
