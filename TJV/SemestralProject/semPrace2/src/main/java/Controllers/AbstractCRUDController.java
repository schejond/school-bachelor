package Controllers;

import java.util.Collection;
import java.util.Objects;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

public abstract class AbstractCRUDController<T> {

    private final Class<T> entityClass;
    
    @PersistenceContext
    private EntityManager entityManager;

    protected AbstractCRUDController(Class<T> entityClass) {
        this.entityClass = entityClass;
    }

    protected abstract Object getEntityId(T e);

    public void create(T e) {
        entityManager.persist(Objects.requireNonNull(e));
    }

    protected T retrieveEntity(Object id) {
        return entityManager.find(entityClass, id);
    }

    public T retrieve(Object id) {
        return retrieveEntity(Objects.requireNonNull(id));
    }

    public Collection<T> retrieveAll() {
        return entityManager
                .createQuery(
                        entityManager.getCriteriaBuilder().createQuery(entityClass)
                ).getResultList();
    }

//    protected void updateOrCreateEntity(T e) {
//        entityManager.merge(e);
//    }
//
    public T updateOrCreate(T e) {
        Objects.requireNonNull(e);
        return entityManager.merge(e);
    }

    public T delete(Object id) {
        T e = entityManager.find(entityClass, Objects.requireNonNull(id));
        if (e != null) {
            entityManager.remove(e);
        }
        return e;
    }

    public boolean exists(T e) {
        return entityManager
                .find(entityClass,
                        getEntityId(Objects.requireNonNull(e))
                ) != null;
    }

    public boolean existsById(Object id) {
        return entityManager.find(entityClass, Objects.requireNonNull(id)) != null;
    }
}
