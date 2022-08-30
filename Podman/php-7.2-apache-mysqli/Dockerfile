FROM php:7.1.23-apache
RUN docker-php-ext-install mysqli && docker-php-ext-enable mysqli
CMD ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]
