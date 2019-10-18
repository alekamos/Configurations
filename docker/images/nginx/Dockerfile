#GET the base default nginx image from docker hub
FROM nginx:alpine

#Delete the Existing default.conf
RUN rm /etc/nginx/conf.d/default.conf 

# install nano and curl
RUN apk add nano && apk add curl
 
#Copy the custom default.conf to the nginx configuration
COPY default.conf /etc/nginx/conf.d
COPY localhost.cert localhost.key /etc/ssl/
