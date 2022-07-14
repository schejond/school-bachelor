package cz.cvut.fit.tjv.cv.semprace2.semprace2restclient;

import java.util.function.Function;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.WebTarget;

public class RestClient {
    public static final String BACKEND_REST_URL = "http://localhost:8080/semPrace2/app";
    private static final ClientBuilder BUILDER = ClientBuilder.newBuilder();
    
    private static Client getClientInstance() {
        return BUILDER.build();
    }
    
    private static WebTarget getTarget(Client c, String path) {
        return c.target(BACKEND_REST_URL).path(path);
    }
    
    public static <V> V processRequest(String resourcePath, Function<WebTarget, V> request) {
        final Client c = RestClient.getClientInstance();
        try {
            return request.apply(getTarget(c, resourcePath));
        } catch (WebApplicationException e) {
            throw new SemestralkaAppException(e);
        } finally {
            c.close();
        }
    }   
}
