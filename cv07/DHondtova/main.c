#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int n;
    printf("Pocet stran:\n");
    if(scanf("%d", &n) != 1 || n < 2 || n > 26)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }

    char str;
    long long vote;
    unsigned long long voteSum = 0;
    unsigned long long *votes = (unsigned long long *)calloc(n, sizeof(vote));

    printf("Strany a pocet hlasu:\n");
    for(int i = 0; i < n; i++)
    {
        if(scanf("\n%c %lld", &str, &vote) != 2 || str < 'A' || str >= 'A' + n || votes[(int)(str-'A')] || vote <= 0)
        {
            printf("Nespravny vstup.\n");
            free(votes);
            return 0;
        }
        votes[(int)(str-'A')] = vote;
        voteSum += vote;
    }
    for(int i = 0; i < n; i++)
    {
        if( !votes[i] )
        {
            printf("Nespravny vstup.\n");
            free(votes);
            return 0;
        }
    }

    long long M, assigned = 0;
    printf("Pocet horniku:\n");
    if(scanf("%lld", &M) != 1 || M < 0)
    {
        printf("Nespravny vstup.\n");
        free(votes);
        return 0;
    }

    unsigned long long *miners = (unsigned long long *)malloc( n * sizeof(unsigned long long) ); 
    for(int i = 0; i < n; i++)
    {
        miners[i] = votes[i] * (unsigned long long)M / voteSum;
        if(miners[i]) miners[i]--;
        assigned += miners[i];
    }

    int maxIndex = 0, maxCnt = 0;
    long double max_pref, pref;
    while(assigned < M)
    {   
        max_pref = 0;
        for(int i = 0; i < n; i++)
        {
            pref = (long double)votes[i] / ( miners[i] ? ( miners[i] + 1 ) : 1.42 );
            if(pref > max_pref)
            {
                max_pref = pref;
                maxIndex = i;
                maxCnt = 1;
            }
            else if( pref == max_pref) maxCnt++;
        }

        if( maxCnt > ( M - assigned ) )
        {
            printf("Nelze rozdelit.\n");
            free(votes);
            free(miners);
            return 0;
        }
        miners[maxIndex]++;
        assigned++;
    }

    printf("Pridelene pocty:\n");
    for(int i = 0; i < n; i++)
    {
        printf("%c: %lld\n", 'A' + (char)i, miners[i]);
    }

    free(votes);
    free(miners);
    return 0;
}


/*

  h_1 | h_2 | h_3 ... h_i ... h_n  sum  H
  p_1 | p_2 | p_3 ... p_i ... p_n
  m'1 | m'2 | m'3 ... m'i ... m'n  sum  M'
   ^  |  ^  |  ^       ^       ^   
  m_1 | m_2 | m_3 ... m_i ... m_n  sum  M

    P >= max( p_i )

           h_i
    m'i = ----- - 1 <= m_i
            P

    h'i = h_i / H

       H            h'i                     
    ------- = sum( ----- )^-1 <= pi( p_i^h'i ) <= sum( p_i*h'i ) <= sqrt( sum( h'i^2 ) * sum( p_i^2 ) )
     M + n          p_i                     
                           HM <= GM         GM <= AM     Cauchy-Schwarz

    sqrt( sum( h'i^2 ) * sum( p_i^2 ) ) <= sqrt( n * sum( h'i^2 ) ) * max( p_i )

*/
