/*
typedef struct tagLIST LIST;
struct tagLIST
{
  int Data;
  LIST *Next;
};

typedef struct
{
  LIST *Top;
} STACK;

int Push( STACK *S, int NewData )
{
  LIST *NewElement;

  NewElement = malloc(sizeof(LIST));
  if (NewElement == NULL)
    return 0;

  NewElement->Data = NewData;
  NewElement->Next = S->Top;

  S->Top = NewElement;
  return 1;
}

int Pop( STACK *S, int *OldData )
{
  if (S->Top == NULL)
    return 0;

  *OldData = S->Top->Data;

  S->Top = S->Top->Next;
  return 1;
}
*/