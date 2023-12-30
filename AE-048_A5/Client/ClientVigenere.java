import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class ClientVigenere {
    public static void main(String[] args) {
        try {
            // Get reference to the remote server object from the registry
            Registry registry = LocateRegistry.getRegistry("localhost", 1099);
            RemoteServerInterface remoteServer = (RemoteServerInterface) registry.lookup("RemoteServer");

            String keyword = "GORDIAN";  // Replace with the actual Vigenere cipher keyword
            String decryptedMessage = remoteServer.decryptVigenere(keyword);

            System.out.println("Decrypted Message (Vigenere): " + decryptedMessage);
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
