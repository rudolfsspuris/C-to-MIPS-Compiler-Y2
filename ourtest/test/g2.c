int f(int a, int c){
    if(a>0){
        c = f(a-1, c);
        c++;
        return c;
    }
    else{
        c++;
        return c;
    }
}
