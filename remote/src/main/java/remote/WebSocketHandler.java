package remote;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.jetty.websocket.api.Session;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketClose;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketConnect;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketMessage;
import org.eclipse.jetty.websocket.api.annotations.WebSocket;

@WebSocket
public class WebSocketHandler {
    static List<Session> sessions = new ArrayList<>();

    @OnWebSocketConnect
    public void onConnect(Session user) throws Exception {
    	//sessions.add(user);
    }

    @OnWebSocketClose
    public void onClose(Session user, int statusCode, String reason) {
        
    }

    @OnWebSocketMessage
    public void onMessage(Session user, String message) {
    	synchronized (sessions) {
    		sessions.add(user);
		}
    }
    
    public static void broadcastMessage(final String message) {
    	synchronized (sessions) {
            sessions.stream().filter(Session::isOpen).parallel().forEach(session -> {
                try {
                    session.getRemote().sendString(message);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            });
        	sessions.clear();
		}
    }
}
