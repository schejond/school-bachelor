package Rest;

import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import javax.ws.rs.ApplicationPath;
import javax.ws.rs.core.Application;

@ApplicationPath("/app")
public class semestralkaEndpoint extends Application {

    @Override
    public Set<Class<?>> getClasses() {
        return Stream
                .of(ContactResource.class, CustomerResource.class, OrderResource.class)
                .collect(Collectors.toSet());
    }
    
}
