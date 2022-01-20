typedef struct node NodeType;
struct node { //Node Struct
        int num;
        NodeType *next;
   };
typedef struct { //list struct
        int length;
        NodeType *head;
    } List;
void init(List *calls) ;
void add(List *calls, int i) ;
void print(List calls) ;
int length(List calls) ;
int check (List L, int num) ;
char* getPrefix (int num) ;