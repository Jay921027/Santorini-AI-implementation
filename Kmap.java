// Do not use public classes to prevent from compile errors
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Scanner;

class Kmap {
    // PLEASE DO NOT MODIFY THE SOLUTION CLASS
    static class Solution {
        public int numVar;
        public char[][] kmap;
        public ArrayList<String> primes;
        public ArrayList<String> essentials;
        public ArrayList<String> minimumSop;
    }

    // PLEASE DO NOT MODIFY THE FUNCTION DEFINITION
    public static Solution solveKmap(int numVar, ArrayList<Integer> minterms, ArrayList<Integer> dontcares) {
        Solution sol = new Solution();
        sol.kmap = new char[4][4];
        sol.primes = new ArrayList<>();
        sol.essentials = new ArrayList<>();
        sol.minimumSop = new ArrayList<>();
        ArrayList<Integer> implicants = new ArrayList<>();
        ArrayList<Integer> prime_implicants = new ArrayList<>();
        ArrayList<Integer> essential_prime_implicants = new ArrayList<>();
        char big_kmap[][] = new char[5][5]; // In order to scan the corner implicants of kmap
        int nRow = 0, nCol = 0;
        int index[] = get_index(numVar); // Get nRow and nCol
        nRow = index[0];
        nCol = index[1];
        int m = minterms.size(); // To minimize runtime
        int d = dontcares.size();
        // Set up kmap with all zeros
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nCol; j++) {
                sol.kmap[i][j] = '0';
            }
        }
        // Fill in 1 and dontcare under different numVar of kmap
        switch (numVar) {
            case 2:
                sol.numVar = 2;
                for (int i = 0; i < m; i++) {
                    switch(minterms.get(i)) {
                        case 0:
                            sol.kmap[0][0] = '1';
                            break;
                        case 1:
                            sol.kmap[1][0] = '1';
                            break;
                        case 2:
                            sol.kmap[0][1] = '1';
                            break;
                        case 3:
                            sol.kmap[1][1] = '1';
                            break;
                    }
                }
                for (int i = 0; i < d; i++) {
                    switch(dontcares.get(i)) {
                        case 0:
                            sol.kmap[0][0] = 'x';
                            break;
                        case 1:
                            sol.kmap[1][0] = 'x';
                            break;
                        case 2:
                            sol.kmap[0][1] = 'x';
                            break;
                        case 3:
                            sol.kmap[1][1] = 'x';
                            break;
                    }
                }
                break;
            case 3:
                sol.numVar = 3;
                for (int i = 0; i < m; i++) {
                    switch(minterms.get(i)) {
                        case 0:
                            sol.kmap[0][0] = '1';
                            break;
                        case 1:
                            sol.kmap[1][0] = '1';
                            break;
                        case 2:
                            sol.kmap[0][1] = '1';
                            break;
                        case 3:
                            sol.kmap[1][1] = '1';
                            break;
                        case 4:
                            sol.kmap[0][3] = '1';   
                            break;
                        case 5:
                            sol.kmap[1][3] = '1';
                            break;
                        case 6:
                            sol.kmap[0][2] = '1';
                            break;
                        case 7:
                            sol.kmap[1][2] = '1';
                            break;
                    }
                }
                for (int i = 0; i < d; i++) {
                    switch(dontcares.get(i)) {
                        case 0:
                            sol.kmap[0][0] = 'x';
                            break;
                        case 1:
                            sol.kmap[1][0] = 'x';
                            break;
                        case 2:
                            sol.kmap[0][1] = 'x';
                            break;
                        case 3:
                            sol.kmap[1][1] = 'x';
                            break;
                        case 4:
                            sol.kmap[0][3] = 'x';   
                            break;
                        case 5:
                            sol.kmap[1][3] = 'x';
                            break;
                        case 6:
                            sol.kmap[0][2] = 'x';
                            break;
                        case 7:
                            sol.kmap[1][2] = 'x';
                            break;
                    }
                }
                break;
            case 4:
                sol.numVar = 4;
                for (int i = 0; i < m; i++) {
                    switch(minterms.get(i)) {
                        case 0:
                            sol.kmap[0][0] = '1';
                            break;
                        case 1:
                            sol.kmap[1][0] = '1';
                            break;
                        case 2:
                            sol.kmap[3][0] = '1';
                            break;
                        case 3:
                            sol.kmap[2][0] = '1';
                            break;
                        case 4:
                            sol.kmap[0][1] = '1';   
                            break;
                        case 5:
                            sol.kmap[1][1] = '1';
                            break;
                        case 6:
                            sol.kmap[3][1] = '1';
                            break;
                        case 7:
                            sol.kmap[2][1] = '1';
                            break;
                        case 8:
                            sol.kmap[0][3] = '1';
                            break;
                        case 9:
                            sol.kmap[1][3] = '1';
                            break;
                        case 10:
                            sol.kmap[3][3] = '1';   
                            break;
                        case 11:
                            sol.kmap[2][3] = '1';
                            break;
                        case 12:
                            sol.kmap[0][2] = '1';
                            break;
                        case 13:
                            sol.kmap[1][2] = '1';
                            break;
                        case 14:
                            sol.kmap[3][2] = '1';
                            break;
                        case 15:
                            sol.kmap[2][2] = '1';
                            break;
                    }
                }
                for (int i = 0; i < d; i++) {
                    switch(dontcares.get(i)) {
                        case 0:
                            sol.kmap[0][0] = 'x';
                            break;
                        case 1:
                            sol.kmap[1][0] = 'x';
                            break;
                        case 2:
                            sol.kmap[3][0] = 'x';
                            break;
                        case 3:
                            sol.kmap[2][0] = 'x';
                            break;
                        case 4:
                            sol.kmap[0][1] = 'x';   
                            break;
                        case 5:
                            sol.kmap[1][1] = 'x';
                            break;
                        case 6:
                            sol.kmap[3][1] = 'x';
                            break;
                        case 7:
                            sol.kmap[2][1] = 'x';
                            break;
                        case 8:
                            sol.kmap[0][3] = 'x';
                            break;
                        case 9:
                            sol.kmap[1][3] = 'x';
                            break;
                        case 10:
                            sol.kmap[3][3] = 'x';   
                            break;
                        case 11:
                            sol.kmap[2][3] = 'x';
                            break;
                        case 12:
                            sol.kmap[0][2] = 'x';
                            break;
                        case 13:
                            sol.kmap[1][2] = 'x';
                            break;
                        case 14:
                            sol.kmap[3][2] = 'x';
                            break;
                        case 15:
                            sol.kmap[2][2] = 'x';
                            break;
                    }
                }
                break;
        }
        // Check if the minimum SOP is 0 (all 0s or all dontcares)          
        int allzero = 1;
        for(int i = 0; i < nRow && allzero == 1; i++) {
            for (int j = 0; j < nCol && allzero == 1; j++) {
                if (sol.kmap[i][j] == '1') allzero = 0;
            }
        }
        if (allzero == 1) {
            sol.minimumSop.add("0");
            return sol;
        }
        // Check if the minimum SOP is 1 (all 1s)
        int allone = 1;
        for(int i = 0; i < nRow && allone == 1; i++) {
            for (int j = 0; j < nCol && allone == 1; j++) {
                if (sol.kmap[i][j] == '0') allone = 0;
            }
        }
        if (allone == 1) {
            sol.primes.add("1");
            sol.essentials.add("1");
            sol.minimumSop.add("1");
            return sol;
        }
        // Convert kmap to big_kmap
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nCol; j++) {
                big_kmap[i][j] = sol.kmap[i][j];
            }
        }
        for (int i = 0; i < nRow; i++) {
            big_kmap[i][nCol] = sol.kmap[i][0];
        }
        for (int j = 0; j < nCol; j++) {
            big_kmap[nRow][j] = sol.kmap[0][j];
        }
        big_kmap[nRow][nCol] = sol.kmap[0][0];

        // Find all possible implicants        
        find_all_implicants(sol.numVar, big_kmap, sol.kmap, implicants);
        // Find the prime implicants from all implicants
        find_prime_implicants(implicants, prime_implicants);
        ArrayList<Integer> bestsol = new ArrayList<>(prime_implicants); // Represent the minimum SOP in bits
        int target = 0; // Represent all 1s of original kmap in bits, our goal is to completely fill in this int by bit operations and get the minimum SOP
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nCol; j++) {
                if (sol.kmap[i][j] == '1') {
                    target |= (1 << get_minterm(i, j, numVar));
                }
            }
        }
        ArrayList<Integer> currentsol = new ArrayList<>(); 
        int set_of_dontcares = 0; // Represent all dontcares of original kmap in bits
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nCol; j++) {
                if (sol.kmap[i][j] == 'x') {
                    set_of_dontcares |= (1 << get_minterm(i, j, numVar));
                }
            }
        }
        find_essentail_prime_implicants(prime_implicants, essential_prime_implicants, set_of_dontcares);
        find_minimum_SOP(target, 0, 0, prime_implicants, bestsol, currentsol);
        // Convert ArrayList<Integer> which contains each term in bits to ArrayList<String> 
        print_implicants(sol.numVar, prime_implicants, sol.primes);
        print_implicants(sol.numVar, essential_prime_implicants, sol.essentials);
        print_implicants(sol.numVar, bestsol, sol.minimumSop);
        
        return sol;
    }

    public static void find_all_implicants(int numVar, char big_kmap[][], char kmap[][], ArrayList<Integer> implicants) {
        int nRow = 0, nCol = 0;
        int index[] = get_index(numVar);
        nRow = index[0];
        nCol = index[1];
        int newimplicant = 0;

        // Search for 1x1 implicants
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nCol; j++) {
                newimplicant = 0;
                if (kmap[i][j] == '1') {
                    newimplicant |= (1 << get_minterm(i, j, numVar));
                    implicants.add(newimplicant);
                }
            }
        }        
        // Search for 1x2 implicants
        find_single_implicants(numVar, implicants, nRow, nCol, big_kmap, 1, 2);
        // Search for 1x4 implicants
        if (numVar >= 3) {
            find_single_implicants(numVar, implicants, nRow, nCol, big_kmap, 1, 4);
        }
        // Search for 2x1 implicants
        find_single_implicants(numVar, implicants, nRow, nCol, big_kmap, 2, 1);
        // Search for 2x2 implicants
        find_single_implicants(numVar, implicants, nRow, nCol, big_kmap, 2, 2);
        // Search for 2x4 implicants (in 3 variables case, the minimum SOP will be 1)
        if (numVar >= 4) {
            find_single_implicants(numVar, implicants, nRow, nCol, big_kmap, 2, 4);
        }
        // Search for 4x1 implicants
        if (numVar >= 4) {
            find_single_implicants(numVar, implicants, nRow, nCol, big_kmap, 4, 1);
        }
        // Search for 4x2 implicants
        if (numVar >= 4) {
            find_single_implicants(numVar, implicants, nRow, nCol, big_kmap, 4, 2);
        }
        // Need not to search for 4x4 implicants, because the minimum SOP will be 1
    }

    public static int[] get_index(int numVar) {
        int nRow = 0, nCol = 0;
        switch (numVar) {
            case 2:
                nRow = 2; nCol = 2;
                break;
            case 3:
                nRow = 2; nCol = 4;
                break;
            case 4:
                nRow = 4; nCol = 4;
                break;
        }
        int result[] = {nRow, nCol};
        return result;
    }

    // Convert the indexs of kmap array to decimal minterm
    public static int get_minterm(int i, int j, int numVar) {
        int shift[] = {0, 1, 3, 2, 0};
        int shift_2[] = {0, 1, 0};
        if (numVar == 4) {
            return shift[i] + 4 * shift[j];
        }
        else if (numVar == 3) {
            return shift[i] + 2 * shift[j];
        }
        else return shift_2[i] + 2 * shift_2[j];
    }

    // Find axb single implicants
    public static void find_single_implicants(int numVar, ArrayList<Integer> implicants, int nRow, int nCol, char big_kmap[][], int a, int b) {
        int allone = 1;
        int alldont = 1;
        int newimplicant = 0;
        // If the size of the implicant is 1*j or i*1, or the length/width of implicant is equal to nRow/nCol, avoid duplicate implicants is necessary.
        if(a == 1 || a == nRow){
            nRow--;
        }
        if(b == 1 || b == nCol) {
            nCol--;
        }
        for (int i = 0; i < nRow + 2 - a ; i++) {
            for (int j = 0; j < nCol + 2 - b ; j++) {
                allone = 1;
                alldont = 1;
                newimplicant = 0;
                for (int m = 0; m < a && allone == 1; m++) {
                    for (int n = 0; n < b && allone == 1; n++) {
                        if (big_kmap[i + m][j + n] == '0') allone = 0;
                        if (big_kmap[i + m][j + n] == '1') alldont = 0;
                    }
                }
                if (allone == 1 && alldont == 0) {
                    for (int m = 0; m < a; m++) {
                        for (int n = 0; n < b; n++) {
                            newimplicant |= (1 << get_minterm(i + m, j + n, numVar)); // Input the minterm to corresponding digit in bits
                        }
                    }
                    implicants.add(newimplicant);
                }
            }
        }
    }

    public static void find_prime_implicants(ArrayList<Integer> implicants, ArrayList<Integer> prime_implicants) {
        int p = 0;
        if (implicants.size() == 1) { // Only one implicant
            prime_implicants.add(implicants.get(0));  
            return;
        }
        int i_s = implicants.size();
        // Use and operator to verify if implicant.get(i) is a prime implicant
        for (int i = 0; i < i_s; i++) {
            for (int j = 0; j < i_s; j++) {
                if (i == j) continue;
                p = implicants.get(i) & implicants.get(j);
                if (p == implicants.get(i)) break;
            }
            if (p != implicants.get(i)) { // if no other implicant is contained in implicant.get(i), it's a prime implicant
                prime_implicants.add(implicants.get(i));  
            }
        }
    }

    public static void find_essentail_prime_implicants(ArrayList<Integer> prime_implicants, ArrayList<Integer> essential_prime_implicants, int set_of_dontcares) {
        int p = 0;
        if (prime_implicants.size() == 1) { // Only one prime implicant
            essential_prime_implicants.add(prime_implicants.get(0));  
            return;
        }
        int pi_s = prime_implicants.size();
        // Use or operator to verify if prime_implicant.get(i) is a essential implicant
        for (int i = 0; i < pi_s; i++) {
            p = ~prime_implicants.get(i) | set_of_dontcares;
            for (int j = 0; j < pi_s; j++) {
                if (i == j) continue;
                p |= prime_implicants.get(j);
            }
            if (p != 0xffffffff) {
                essential_prime_implicants.add(prime_implicants.get(i));  
            }
        }
    }

    public static void find_minimum_SOP(int target, int current, int level, ArrayList<Integer> prime_implicants, ArrayList<Integer> bestsol, ArrayList<Integer> currentsol) {
        if ((current & target) == target) { // if current is 
            if (currentsol.size() < bestsol.size()) { // if the number of terms in currentsol is smaller than the bestsol's, currentsol may be the possible minimum SOP
                bestsol.clear();
                bestsol.addAll(currentsol);
            }
            return;
        }
        if (level >= prime_implicants.size()) return; // If all implicants have been considered, end the recursion
        
        // Suppose prime_implicants.get(level) is in the minimum SOP
        currentsol.add(prime_implicants.get(level));
        find_minimum_SOP(target, current | prime_implicants.get(level), level + 1, prime_implicants, bestsol, currentsol);
        // Suppose prime_implicants.get(level) is not in the minimum SOP
        currentsol.remove(prime_implicants.get(level));
        find_minimum_SOP(target, current, level + 1, prime_implicants, bestsol, currentsol);
        return;
    }

    public static void print_implicants(int numVar, ArrayList<Integer> input, ArrayList<String> output) {
        int alphacode[][] = new int[4][2];
        String alphabet[][] = {{"a", "a'"}, {"b", "b'"}, {"c", "c'"}, {"d", "d'"}};
        switch (numVar) {
            case 2:
                alphacode[0][0] = 0x0000000c; // The bits contain all implicants of A
                alphacode[0][1] = 0x00000003; // The bits contain all implicants of A'
                alphacode[1][0] = 0x0000000A; // The bits contain all implicants of B
                alphacode[1][1] = 0x00000005; // The bits contain all implicants of B'
                break;
            case 3:
                alphacode[0][0] = 0x000000f0; // A
                alphacode[0][1] = 0x0000000f; // A'
                alphacode[1][0] = 0x000000cc; // B
                alphacode[1][1] = 0x00000033; // B'
                alphacode[2][0] = 0x000000AA; // C
                alphacode[2][1] = 0x00000055; // C'
                break;
            case 4:
                alphacode[0][0] = 0x0000ff00; // A
                alphacode[0][1] = 0x000000ff; // A'
                alphacode[1][0] = 0x0000f0f0; // B
                alphacode[1][1] = 0x00000f0f; // B'
                alphacode[2][0] = 0x0000cccc; // C
                alphacode[2][1] = 0x00003333; // C'
                alphacode[3][0] = 0x0000AAAA; // D
                alphacode[3][1] = 0x00005555; // D'
                break;
        }
        String str = "";
        int i_s = input.size();
        for (int i = 0; i < i_s; i++) {
            str = "";
            for (int j = 0; j < numVar; j++) {
                if ((input.get(i) & alphacode[j][0]) == 0 || (input.get(i) & alphacode[j][1]) == 0) { // If X is dependent on A/A', one of XA and XA' must be 0
                    if ((input.get(i) & alphacode[j][0]) != 0) { // If X contains A, XA != 0
                        str += alphabet[j][0]; 
                    }
                    else str += alphabet[j][1];
                }
            }
            if (!str.equals("")) output.add(str);
        }
    }
    public static void main (String args[]) {
        ArrayList<Integer> min = new ArrayList<>();
        ArrayList<Integer> dont = new ArrayList<>();
        int numVar = 0;
        Scanner input;
        while(true) {
            System.out.println("Please enter numVar:");
            input = new Scanner(System.in);
            String num = input.nextLine(); 
            try {
                numVar = Integer.parseInt(num);
                if (numVar < 2|| numVar > 4) System.out.println("Invalid input, please try again.");
                else break;
            } catch (Exception e) {
                System.out.println("Invalid input, please try again.");
            }
        }

        int index[] = get_index(numVar); // Get nRow and nCol
        int nRow = index[0];
        int nCol = index[1];

        int minvalid = 0;
        int no_min = 0;
        String minarr[] = {};
        while(minvalid == 0) {
            System.out.println("Please enter minterm:");
            input = new Scanner(System.in);
            String num = input.nextLine(); 
            minarr = num.split(" ");
            minvalid = 1;
            no_min = 0;
            
            try {
                if (minarr[0].equals("x")) no_min = 1;
                for (int i = 0; i < minarr.length && no_min == 0; i++) {
                    if (Integer.parseInt(minarr[i]) < 0 || Integer.parseInt(minarr[i]) > nRow * nCol - 1) {
                        System.out.println("Invalid input, please try again.");
                        minvalid = 0;
                        break;
                    }
                }
            } catch (Exception e) {
                System.out.println("Invalid input, please try again.");
            }
        }

        String dontarr[] = {};
        int pass = 0;
        int no_dont = 0;
        while(pass == 0 && minarr.length < nRow * nCol) {
            System.out.println("Please enter dontcare:");
            input = new Scanner(System.in);
            String num = input.nextLine(); 
            dontarr = num.split(" ");
            pass = 1;
            int breakloop = 0;
            no_dont = 0;

            try {
                if (dontarr[0].equals("x") && no_min == 0) no_dont = 1;
                for (int i = 0; i < dontarr.length && breakloop == 0 && no_dont == 0; i++) {
                    if (Integer.parseInt(dontarr[i]) < 0 || Integer.parseInt(dontarr[i]) > nRow * nCol - 1) {
                        System.out.println("Invalid input, please try again.");
                        pass = 0;
                        break;
                    }
                    else {
                        for (int j = 0; j < minarr.length; j++) {
                            if (no_dont == 1 || no_min == 1) {
                                breakloop = 1;
                                break;
                            }
                            if ((Integer.parseInt(dontarr[i]) == Integer.parseInt(minarr[j]))) {
                                System.out.println("Some dontcares are already included in minterms.");
                                pass = 0;
                                breakloop = 1;
                                break;
                            }
                        }   
                    }
                }
            } catch (Exception e) {
                System.out.println("Invalid input, please try again.");
                pass = 0;
            }
        }
        input.close();
        if (no_min == 0) {
            for (int i = 0; i < minarr.length; i++) {
                min.add(Integer.parseInt(minarr[i]));
            }
        }
        if (no_dont == 0) {
            for (int i = 0; i < dontarr.length; i++) {
                dont.add(Integer.parseInt(dontarr[i]));
            }
        }

        printKmapSolution(solveKmap(numVar, min, dont));
    }

    public static void printKmapSolution(Kmap.Solution sol) {
        int nRow = 0, nCol = 0;
        switch (sol.numVar) {
            case 2:
                nRow = 2; nCol = 2;
                break;
            case 3:
                nRow = 2; nCol = 4;
                break;
            case 4:
                nRow = 4; nCol = 4;
                break;
        }
        System.out.println("K-map:");
        for (int j = 0; j < nCol; j++) {
            System.out.printf("______");
        }
        System.out.println("_");
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nCol; j++) {
                System.out.printf("|     ");
            }
            System.out.println("|");
            for (int j = 0; j < nCol; j++) {
                System.out.printf("|  %c  ", sol.kmap[i][j]);
            }
            System.out.println("|");
            for (int j = 0; j < nCol; j++) {
                System.out.printf("|_____");
            }
            System.out.println("|");            
        }   
        System.out.print("\nPrime implicants: ");
        for (int i = 0; i < sol.primes.size(); i++) {
            System.out.print(sol.primes.get(i));
            if (i != sol.primes.size() - 1) System.out.print(", ");
        }
        System.out.println();
        System.out.print("Essential prime implicants: ");
        for (int i = 0; i < sol.essentials.size(); i++) {
            System.out.print(sol.essentials.get(i));
            if (i != sol.essentials.size() - 1) System.out.print(", ");
        }
        System.out.println();
        System.out.print("Minimum SOP: ");
        for (int i = 0; i < sol.minimumSop.size(); i++) {
            System.out.print(sol.minimumSop.get(i));
            if (i != sol.minimumSop.size() - 1) System.out.print(" + ");
        }
        System.out.println();
    }
}