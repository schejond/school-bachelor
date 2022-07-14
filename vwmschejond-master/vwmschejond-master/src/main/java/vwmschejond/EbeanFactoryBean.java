package vwmschejond;

import io.ebean.EbeanServer;
import io.ebean.EbeanServerFactory;
import io.ebean.config.ServerConfig;
import org.springframework.beans.factory.FactoryBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.io.BufferedInputStream;
import java.io.InputStreamReader;

@Component
public class EbeanFactoryBean implements FactoryBean<EbeanServer> {
    @Override
    public EbeanServer getObject() {
        ServerConfig config = new ServerConfig();
        config.setName("db");
        config.loadFromProperties();
        config.setExplicitTransactionBeginMode(true);
        return EbeanServerFactory.create(config);
    }

    @Override
    public Class<?> getObjectType() {
        return EbeanServer.class;
    }

    @Override
    public boolean isSingleton() {
        return true;
    }
}
