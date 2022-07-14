package Rest;

import Controllers.AbstractCRUDController;
import java.util.Collection;
import java.util.function.Function;
import java.util.stream.Collectors;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.PUT;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

public abstract class AbstractCRUDResource<T, U> {
    
    protected final Function<U, T> dtoToEntityConverter;
    protected final Function<T, U> entityToDtoConverter;

    protected AbstractCRUDResource(Function<U, T> dtoToEntityConverter, Function<T, U> entityToDtoConverter) {
        this.dtoToEntityConverter = dtoToEntityConverter;
        this.entityToDtoConverter = entityToDtoConverter;
    }
    
    protected abstract AbstractCRUDController<T> getController();
    @GET
    @Produces({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
    public Collection<U> retrieveAll() {
        return getController().retrieveAll()
                .stream()
                .map(entityToDtoConverter)
                .collect(Collectors.toList());
    }
    
    protected abstract boolean containsEntityId(U dto);
        
    @PUT
    @Consumes({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
    @Produces({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
    public Response createOrUpdate(U dto) {
        T entity = dtoToEntityConverter.apply(dto);
        boolean update = containsEntityId(dto) && getController().exists(entity);
        entity = getController().updateOrCreate(entity);
        if (update)
            return Response.ok(entityToDtoConverter.apply(entity)).build();
        else
            return Response.status(Response.Status.CREATED)
                    .entity(entityToDtoConverter.apply(entity)).build();
    }
    
    protected Response retrieveById(Object id) {
        if (getController().existsById(id))
            return Response
                    .ok(entityToDtoConverter.apply(getController().retrieve(id)))
                    .build();
        else 
            return Response.noContent().build();
    }
    
    protected Response deleteById(Object id) {
        if (getController().existsById(id)) {
            getController().delete(id);
            return Response.noContent().build();
        } else {
            return Response.status(Response.Status.BAD_REQUEST).build();
        }
    }
}
