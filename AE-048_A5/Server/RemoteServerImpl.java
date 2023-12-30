import java.io.BufferedReader;
import java.io.FileReader;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.IOException;

public class RemoteServerImpl extends UnicastRemoteObject implements RemoteServerInterface {
    private static final long serialVersionUID = 1L;
    private static final String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    public RemoteServerImpl() throws RemoteException {
        // Constructor must throw RemoteException
    }

    private char[][] getTabulaRecta() throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader("TabulaRecta.txt"))) {
            char[][] tabulaRecta;
            tabulaRecta = new char[ALPHABET.length()][ALPHABET.length()];
            String line;

            for (int i = 0; i < ALPHABET.length(); i++) {
                line = reader.readLine();
                tabulaRecta[i] = line.toCharArray();
            }

            return tabulaRecta;
        }
    }
    
    private String getCipherText() throws RemoteException{
        try (BufferedReader reader = new BufferedReader(new FileReader("Cipher.txt"))) {
            StringBuilder stringBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                stringBuilder.append(line);
            }
            return stringBuilder.toString();
        } catch (Exception e) {
            throw new RemoteException("Error reading cipher text file.", e);
        }
    }

    private int findIndexInRow(char[] row, char target) {
        for (int i = 0; i < row.length; i++) {
            if (row[i] == target) {
                return i;
            }
        }
        return -1; // not found
    }

    @Override
    public String decryptCaesar(String keyword) throws RemoteException {
        try {
            // Read the cipher text from the file
            String encryptedText = getCipherText();

            // Load the tabula recta
            char[][] tabulaRecta = getTabulaRecta();

            // Ensure the keyword is in uppercase for consistency
            keyword = keyword.toUpperCase();

            StringBuilder decryptedText = new StringBuilder();

            int keywordIndex = 0;

            for (char c : encryptedText.toCharArray()) {
                if (Character.isLetter(c)) {
                    // Find the corresponding row in the tabula recta for the current keyword letter
                    int rowIndex = ALPHABET.indexOf(keyword.charAt(keywordIndex));

                    // Find the index of the ciphertext letter in the row
                    int colIndex = findIndexInRow(tabulaRecta[rowIndex], c);

                    // Decrypt the letter using the column's corresponding letter in the first row
                    char decryptedChar = ALPHABET.charAt(colIndex);

                    decryptedText.append(decryptedChar);

                    // Move to the next letter in the keyword
                    keywordIndex = (keywordIndex + 1) % keyword.length();
                } else {
                    // Non-letter characters are appended as is
                    decryptedText.append(c);
                }
            }

            return decryptedText.toString();
        } catch (IOException e) {
            throw new RemoteException("Error decrypting with Caesar cipher. Failed to load TabulaRecta.", e);
        } catch (Exception e) {
            throw new RemoteException("Error decrypting with Caesar cipher.", e);
        }
    }

    @Override
    public String decryptVigenere(String keyword) throws RemoteException {
        try {
            // Read the cipher text from the file
            String encryptedText = getCipherText();

            // Ensure the keyword is in uppercase for consistency
            keyword = keyword.toUpperCase();

            StringBuilder decryptedText = new StringBuilder();
            int keyLength = keyword.length();

            for (int i = 0; i < encryptedText.length(); i++) {
                char c = encryptedText.charAt(i);
                char keyChar = keyword.charAt(i % keyLength);

                if (Character.isUpperCase(c)) {
                    decryptedText.append((char) ((c - keyChar + 26) % 26 + 'A'));
                } else if (Character.isLowerCase(c)) {
                    decryptedText.append((char) ((c - keyChar + 26) % 26 + 'a'));
                } else {
                    decryptedText.append(c);
                }
            }

            return decryptedText.toString();
        } catch (Exception e) {
            throw new RemoteException("Error decrypting with Vigenere cipher.", e);
        }
    }

}
