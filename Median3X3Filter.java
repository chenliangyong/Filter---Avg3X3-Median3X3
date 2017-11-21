import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Median3X3Filter {
   private int mirrorFramedAry[][];
   private int tempAry[][];
   private int neighborAry[] = new int[9];
   private int numRows; //image Head info
   private int numCols; //image Head info
   private int minVal;  //image Head info
   private int maxVal;  //image Head info
   private int newMin; 
   private int newMax;
   private String outFile="Median3X3Out.txt";   // default output file name, will be replace by user input
   private String inFile;
   
   public Median3X3Filter(String[] args) throws FileNotFoundException{
      //Get the output filename, if user given a output file name then replace default name;
      if (args.length==2) outFile=args[1];
      inFile=args[0];
      
      msg("Start Reading Head Info...");
      
      //Get the Head info from input file;
      Scanner sc;
      sc = new Scanner(new FileInputStream(inFile));
      if(!sc.hasNext()){
         System.out.println("Error, Empty input file.");
         sc.close();
         return;
      }else{
         numRows=sc.nextInt();
         numCols=sc.nextInt();
         minVal=sc.nextInt();
         newMin=minVal;
         maxVal=sc.nextInt();
         newMax=maxVal;
      }
      sc.close();
      
      //initialize 2D array
      mirrorFramedAry=new int[numRows+2][numCols+2];
      tempAry=new int[numRows+2][numCols+2];
      for (int i = 0; i < numRows + 2; i++) {
         mirrorFramedAry[i] = new int[numCols + 2];
         tempAry[i] = new int[numCols + 2];
         for (int j = 0; j < numCols + 2; j++) {
             mirrorFramedAry[i][j] = 0;
             tempAry[i][j] = 0;
         }
      }
      
      mirrorFramed();
   }
   private void mirrorFramed() throws FileNotFoundException{
      loadImage();
      for (int i = 1; i < numRows + 1 ; i++) {
          mirrorFramedAry[i][0] = mirrorFramedAry[i][1];
          mirrorFramedAry[i][numCols + 1] = mirrorFramedAry[i][numCols];
      }
      for (int j = 0; j < numCols + 2; j++) {
          mirrorFramedAry[0][j] = mirrorFramedAry[1][j];
          mirrorFramedAry[numRows + 1][j] = mirrorFramedAry[numRows][j];
      }
      msg("Finish MirrorFramed...");
      
   }
   private void loadImage() throws FileNotFoundException{
    //Read the original image
      Scanner sc;
      sc = new Scanner(new FileInputStream(inFile));
      if(!sc.hasNext()){
         System.out.println("Error, Empty input file.");
         sc.close();
         return;
      }else{
         String temp=sc.nextLine();
         msg(temp);
         int gVal=0;
         for (int i = 1; i < numRows + 1; i++) {
            for (int j = 1; j < numCols + 1; j++) {
                if (sc.hasNext()) {
                    gVal=sc.nextInt();
                }
                else { break; }
                mirrorFramedAry[i][j] = gVal;
            }
         }
      }
      sc.close();
      msg("Finish Reading Original Image...");
   }
   public void loadNeighbors(){
      for (int i = 1; i < numRows + 1; i++) {
         for (int j = 1; j < numCols + 1; j++) {
             neighborAry[0] = mirrorFramedAry[i - 1][j - 1];
             neighborAry[1] = mirrorFramedAry[i - 1][j];
             neighborAry[2] = mirrorFramedAry[i - 1][j + 1];
             neighborAry[3] = mirrorFramedAry[i][j - 1];
             neighborAry[4] = mirrorFramedAry[i][j];
             neighborAry[5] = mirrorFramedAry[i][j + 1];
             neighborAry[6] = mirrorFramedAry[i + 1][j - 1];
             neighborAry[7] = mirrorFramedAry[i + 1][j];
             neighborAry[8] = mirrorFramedAry[i + 1][j + 1];
             tempAry[i][j] = Median3x3(); // get Median result from 1D Array;
             if (tempAry[i][j] < minVal) newMin = tempAry[i][j];
             if (tempAry[i][j] > maxVal) newMax = tempAry[i][j];
         }   
     }
   }
   private int Median3x3(){
      //Bubble Sort on neighborAry
      for (int i = 0; i < neighborAry.length - 1; i++) {
         for (int j = 0; j < neighborAry.length - i - 1; j++) {  

             if (neighborAry[j] > neighborAry[j + 1]) {  
                 int tmp = neighborAry[j];  
                 neighborAry[j] = neighborAry[j + 1];  
                 neighborAry[j + 1] = tmp;  
             }  
         }  
      }
      return neighborAry[5]; // #5 is the Median number
   }
   public void printResult() throws IOException{
      FileWriter fw = new FileWriter(outFile);
      fw.write(numRows+ " " + numCols + " " + newMin + " " + newMax + "\r\n" ); 
      System.out.println(numRows+ " " + numCols + " " + newMin + " " + newMax);
      for (int i = 1; i < numRows + 1; i++) {
         for (int j = 1; j < numCols + 1; j++) {
            System.out.print(tempAry[i][j]+" ");
            fw.write(tempAry[i][j]+" ");
         }
         System.out.println("");
         fw.write("\r\n");
      }
      fw.close();
   }
   
   public static void main (String[] args){
      try {
         if(args.length < 1){
             System.out.println("Run as : java Media3X3Filter [inputFile1]");
             System.out.println("Run as : java Media3X3Filter [inputFile1] [outputFile1]");
             return;
         }

         Median3X3Filter MedFilter = new Median3X3Filter(args);
         MedFilter.loadNeighbors();
         MedFilter.printResult();
       
      }catch (IOException e) {     
         e.getMessage();
         System.out.println(e.getMessage()+"     ~~~ Try again ~~~");
      }           
   }
   public static void msg(String m) {
      System.out.println("["+m+"]");   
   }
}
