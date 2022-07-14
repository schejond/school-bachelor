package vwmschejond;

import io.ebean.EbeanServer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.io.BufferedInputStream;

@SpringBootApplication
public class TopK {
    public static void main(String[] args) {
        SpringApplication app = new SpringApplication(TopK.class);
        app.run(args);
    }
}
