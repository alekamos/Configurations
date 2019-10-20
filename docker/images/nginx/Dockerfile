#GET the base default nginx image from docker hub
FROM nginx:alpine

#Delete the Existing default.conf
RUN rm /etc/nginx/conf.d/default.conf 

# install nano and curl
RUN apk add nano && apk add curl