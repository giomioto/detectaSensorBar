
#include"gerador_de_testes.h"
#include"imagem.h"
#include"trabalho3.h"
#include<math.h>
#include<stdlib.h>


double detectaSensorBar (Imagem1C* img, Coordenada* l, Coordenada* r)
{
    int*vetor_rotulos;
    int i, j, k;
    int x1_direita, x1_esquerda, y1_cima, y1_baixo, x2_direita, x2_esquerda, y2_cima, y2_baixo;
    int encontrarX1, encontrarX2, encontrarY1, encontrarY2;
    int maior, rotulos, maiorBola1, maiorBola2, menor_rotulo, bola1, bola2;
    int x1, y1, x2, y2;
    double tangente;

//============================================================================================================================//


	for (i=0, maior=0; i< img->altura; i++)
	{
		for (j=0; j < img->largura; j++)
		{
			if (img->dados [i][j] > maior )
            {
                maior = img->dados [i][j];
			}
        }
    }

    maior = (maior)/2;

    for (i=0; i< img->altura; i++)
    {
		for (j=0; j < img->largura; j++)
		{
			if (img->dados [i][j] < maior)
            {
                img->dados [i][j] = 0;
            }
            else if(i==0 || i==img->altura-1 || j==0 || j==img->largura-1)
            {
                 img->dados [i][j] = 0;
            }
			else
            {
                img->dados [i][j] = 255;
			}
        }
    }
    /*static int auxx=0;
    char foostring [64];
    sprintf (foostring, "bin%d.bmp", auxx);
    auxx++;
    salvaImagem1C (img, foostring);*/

//===========================================================================================================================//

    for(i=1, rotulos=1; i< img->altura; i++)
    {
        for(j=1; j< img->largura-1; j++)
        {
            if(img->dados[i][j]==255)
            {
                menor_rotulo = INT_MAX;

                if(img->dados[i][j-1]==0 && img->dados[i-1][j-1]==0 && img->dados[i-1][j]==0 && img->dados[i-1][j+1]==0)
                {
                    img->dados[i][j]=rotulos;
                    rotulos++;
                }

                else
                {
                    if(img->dados[i][j-1] < menor_rotulo && img->dados[i][j-1]!=0)
                    {
                        menor_rotulo = img->dados[i][j-1];
                    }

                    for(k=j-1; k<=j+1; k++)
                    {
                        if(img->dados[i-1][k] < menor_rotulo && img->dados[i-1][k]!=0)
                        {
                            menor_rotulo = img->dados[i-1][k];
                        }
                    }
                    img->dados[i][j] = menor_rotulo;
                }
            }
        }
    }


//===========================================================================================================================//

    for(i=img->altura -2; i > 0; i--)
    {
        for(j=img->largura -2; j > 0; j--)
        {
            if(img->dados[i][j]!=0)
            {
                menor_rotulo = img->dados[i][j];

                if(img->dados[i][j+1] < menor_rotulo && img->dados[i][j+1] != 0)
                {
                    menor_rotulo = img->dados[i][j+1];
                }
                for(k=j-1; k<=j+1; k++)
                {
                    if(img->dados[i+1][k] < menor_rotulo && img->dados[i+1][k] != 0)
                    {
                        menor_rotulo = img->dados[i+1][k];
                    }

                }

                img->dados[i][j] = menor_rotulo;
            }
        }

    }

    /*for(i=0; i<img->altura; i++)
    {
        for(j=0; j<img->largura; j++)
        {
             printf("(%d, %d): %d\n", i, j, img->dados[i][j]);
        }
    }*/

//===========================================================================================================================//
/*    for(i=1; i< img->altura-1; i++)
    {
        for(j=1; j< img->largura-1; j++)
        {
            if(img->dados[i][j]!=0)
            {
                menor_rotulo = INT_MAX;

                for(k=j-1; k<=j+1; k++)
                {
                    if(img->dados[i][j-1] < menor_rotulo && img->dados[i][j-1] != 0)
                    {
                        menor_rotulo = img->dados[i][j-1];
                    }

                    if(img->dados[i-1][k] < menor_rotulo && img->dados[i-1][k] != 0)
                    {
                        menor_rotulo = img->dados[i-1][k];
                    }
                }

                img->dados[i][j] = menor_rotulo;
            }
        }
    }
*/
//===========================================================================================================================//
/*
    for(i=img->altura -2; i > 0; i--)
    {
        for(j=img->largura -2; j > 0; j--)
        {
            if(img->dados[i][j]!=0)
            {
                menor_rotulo = INT_MAX;

                for(k=j-1; k<=j+1; k++)
                {

                    if(img->dados[i][j+1] < menor_rotulo && img->dados[i][j+1] != 0)
                    {
                        menor_rotulo = img->dados[i][j+1];
                    }


                    if(img->dados[i+1][k] < menor_rotulo && img->dados[i+1][k] != 0)
                    {
                        menor_rotulo = img->dados[i+1][k];
                    }

                }

                img->dados[i][j] = menor_rotulo;
            }
        }

    }
*/
//===========================================================================================================================//
    vetor_rotulos = (int*) malloc (sizeof(int)*rotulos);
    for(i=0; i<rotulos; i++)
    {
        vetor_rotulos[i]=0;
    }


    for(i=0; i < img->altura; i++)
    {
        for(j=0; j < img->largura; j++)
        {
            for(k=0; k < rotulos; k++)
            {
                if(img->dados[i][j] == k+1)
                {
                    vetor_rotulos[k]++;
                }
            }
        }
    }

//============================================================================================================================//

    for(i=2, maiorBola1 = vetor_rotulos[0], maiorBola2 = vetor_rotulos[1]; i<rotulos; i++)
    {
        if(maiorBola1 > maiorBola2)
        {
            if(vetor_rotulos[i] > maiorBola2)
            {
                maiorBola2 = vetor_rotulos[i];


            }
        }

        else //(maiorBola1 < maiorBola2)
        {
            if(vetor_rotulos[i] > maiorBola1)
            {
                maiorBola1 = vetor_rotulos[i];


            }
        }
    }
    printf("%d   %d\n", maiorBola1, maiorBola2);

    for(i=0; i< rotulos; i++)
    {
        if(vetor_rotulos[i] == maiorBola1)
        {
            bola1 = i + 1;
        }

        if(vetor_rotulos[i] == maiorBola2)
        {
            bola2 = i + 1;
        }
    }

    free(vetor_rotulos);
//============================================================================================================================//
/*

    for(i=0; i < img->altura; i++)
    {
        for(j=0; j < img->largura; j++)
        {
            if(img->dados[i][j] == bola1 || img->dados[i][j] == bola2)
            {
                img->dados[i][j] = 255;
            }
        }
    }

    static int auxxx=0;
    char foostrings [64];
    sprintf (foostrings, "rot%d.bmp", auxxx);
    auxxx++;
    salvaImagem1C (img, foostrings);

*/
//============================================================================================================================//

    for (i=0, encontrarY1=1, encontrarY2=1; i< img->altura; i++)
	{
		for (j=0; j < img->largura; j++)
        {
            if(encontrarY1==1)
            {
                if(img->dados[i][j] == bola1)
                {
                    y1_cima = i;
                    encontrarY1 = 0;
                }
            }

            if(encontrarY2==1)
            {
                if(img->dados[i][j] == bola2)
                {
                    y2_cima = i;
                    encontrarY2 = 0;
                }
            }
        }

	}

//============================================================================================================================//

    for (i = img->altura -1, encontrarY1=1, encontrarY2=1; i>=0; i--)
	{
		for (j=0; j < img->largura; j++)
        {
            if(encontrarY1==1)
            {
                if(img->dados[i][j] == bola1)
                {
                    y1_baixo = i;
                    encontrarY1 = 0;
                }
            }

            if(encontrarY2==1)
            {
                if(img->dados[i][j] == bola2)
                {
                    y2_baixo = i;
                    encontrarY2 = 0;
                }
            }
        }

	}

//============================================================================================================================//

    for(j=0, encontrarX1=1, encontrarX2=1; j<img->largura; j++)
    {
        for(i=0; i < img->altura; i++)
        {
            if(encontrarX1==1)
            {
                if(img->dados[i][j] == bola1)
                {
                    x1_esquerda = j;
                    encontrarX1 = 0;
                }
            }

            if(encontrarX2==1)
            {
                if(img->dados[i][j] == bola2)
                {
                    x2_esquerda = j;
                    encontrarX2 = 0;
                }
            }
        }
    }

//============================================================================================================================//

    for(j=img->largura -1, encontrarX1=1, encontrarX2=1; j>=0; j--)
    {
        for(i=0; i<img->altura; i++)
        {
            if(encontrarX1==1)
            {
                if(img->dados[i][j] == bola1)
                {
                    x1_direita = j;
                    encontrarX1 = 0;
                }
            }

            if(encontrarX2==1)
            {
                if(img->dados[i][j] == bola2)
                {
                    x2_direita = j;
                    encontrarX2 = 0;
                }
            }
        }
    }

//============================================================================================================================//



    x1 = (x1_direita + x1_esquerda)/2;
    x2 = (x2_direita + x2_esquerda)/2;
    y1 = (y1_cima + y1_baixo)/2;
    y2 = (y2_cima + y2_baixo)/2;








//============================================================================================================================//

    if(x1<x2)
    {
        l->x = x1;
        l->y = y1;
        r->x = x2;
        r->y = y2;

        tangente = atan((double)(y2-y1)/(x2-x1));
        return atan(tangente);
    }

    else
    {
        l->x = x2;
        l->y = y2;
        r->x = x1;
        r->y = y1;

        tangente = atan((double)(y1-y2)/(x1-x2));
        return atan(tangente);
    }

}
