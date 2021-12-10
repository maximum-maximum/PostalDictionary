#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 13

struct cell
{
    char *p_code;  /* 郵便番号 */
    char *address; /* 住所 */
    struct cell *p_next;
    struct cell *a_next;
};

struct cell *hashtable1[HASHSIZE];
struct cell *hashtable2[HASHSIZE];

int hash(char *n)
{
    int i = atoi(n);
    return (i % HASHSIZE);
}

int hash2(char *n)
{
    int i, s;
    i = 0;
    s = 0;
    while (n[i] != 0)
    {
        s = (s * 137 - n[i]) % HASHSIZE;
        i++;
    }
    return s;
}

void add(char *n1, char *n2)
{
    struct cell *p;
    int hashval1, hashval2;
    char *c, *c2;

    p = malloc(sizeof(struct cell));

    /*郵便番号の追加*/
    c = strdup(n1);
    p->p_code = c;
    hashval1 = hash(n1);              //ハッシュ値を知る
    p->p_next = hashtable1[hashval1]; //そのテーブルの先頭のアドレスをnextに
    hashtable1[hashval1] = p;         //今作成したセルを先頭に

    /*住所の追加*/
    c2 = strdup(n2);
    p->address = c2;
    hashval2 = hash2(n2);
    p->a_next = hashtable2[hashval2];
    hashtable2[hashval2] = p;
}

void search(char *n)
{
    int hashval1, hashval2;
    struct cell *ptr;

    hashval1 = hash(n);  //郵便番号のハッシュ値
    hashval2 = hash2(n); //住所のハッシュ値

    ptr = hashtable1[hashval1];
    while (ptr != NULL)
    {
        if (strcmp(ptr->p_code, n) == 0)
        {
            printf("%s\n", ptr->address);
            return;
        }
        ptr = ptr->p_next;
    }

    ptr = hashtable2[hashval2];
    while (ptr != NULL)
    {
        if (strcmp(ptr->address, n) == 0)
        {
            printf("%s\n", ptr->p_code);
            return;
        }
        ptr = ptr->a_next;
    }
    printf("No Data\n");
}

void hash_info()
{
    int i, length;
    struct cell *p;

    for (i = 0; i < HASHSIZE; i++)
    {
        length = 0;
        p = hashtable1[i];

        while (p != NULL)
        {
            length++;
            p = p->p_next;
        }
        printf("hashtable1[%d]:%d\n", i, length);
    }

    printf("---------------------\n");

    for (i = 0; i < HASHSIZE; i++)
    {
        length = 0;
        p = hashtable2[i];

        while (p != NULL)
        {
            length++;
            p = p->a_next;
        }
        printf("hashtable2[%d]:%d\n", i, length);
    }
}

int main(void)
{
    FILE *fp;
    int f;
    int i;
    char buf[1024];
    char buf1[1024];
    char buf2[1024];

    for (i = 0; i < HASHSIZE; i++)
    {
        hashtable1[i] = NULL;
        hashtable2[i] = NULL;
    }

    fp = fopen("post.txt", "r");
    while (1)
    {
        f = fscanf(fp, "%s %s", buf1, buf2);
        if (f == EOF)
            break;
        add(buf1, buf2);
    }
    fclose(fp);

    // ハッシュテーブルの偏りを調べる関数
    // hash_info();

    while (scanf("%s", buf) != EOF)
    {
        search(buf);
    }
    return 0;
}
