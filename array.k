/*
   Array.addAll(Array l0);
   add all list of "l0" at the end of "this" array.
*/
void Array.addAll(String[] list){
    foreach(String l in list){
        this.add(l);
    }
}

/*
// example
void main(String[] args){
    String[] a = ["a","b","c","d"];
    String[] b = ["e","f","g"];

    print a;
    print b;

    a.addAll(b);
    print a;
}
*/
