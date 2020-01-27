int g(){
    int a = 3;
    int b = 0;
    if(1){
        int a = 5;
        b = a;
    }
    return b;
}


int f()
{   int a = 4;
    int b = 8;
    return g();
}
