#!/bin/bash

exec 2>/dev/null    # We dont want any error messages be printed to stdout
trap "response_with_html && exit 0" ERR    # response with an html message when an error occurred and close the script

function response_with_html(){    
    echo -e "Status: 200 OK\r"
    echo -e "Content-type: text/html\r"
    echo -e "\r"
    echo "<!DOCTYPE html>"
    echo "<html><head>"
    echo "<title>456</title>"
    echo "</head><body>"
    echo "<h1>456</h1>"
    echo "<p>Attempt to communicate with the server went wrong.</p>"
    echo "<hr>"
    echo "$SERVER_SIGNATURE"
    echo "</body></html>"
}
        
function response_with_json(){
    echo -e "Status: 200 OK\r"
    echo -e "Content-type: application/json\r"
    echo -e "\r"
    echo "{\"message\": \"Hello World!\"}"
}

if [ "$REQUEST_METHOD" = "POST" ]; then
   
    # The environment variabe $CONTENT_TYPE describes the data-type received
    case "$CONTENT_TYPE" in
    application/json)
        # The environment variabe $CONTENT_LENGTH describes the size of the data
        read -n "$CONTENT_LENGTH" QUERY_STRING_POST        # read datastream 
		
        # The following lines will prevent XSS and check for valide JSON-Data.
        # But these Symbols need to be encoded somehow before sending to this script
        QUERY_STRING_POST=$(echo "$QUERY_STRING_POST" | sed "s/'//g" | sed 's/\$//g;s/`//g;s/\*//g;s/\\//g' )        # removes some symbols (like \ * ` $ ') to prevent XSS with Bash and SQL.
        QUERY_STRING_POST=$(echo "$QUERY_STRING_POST" | sed -e :a -e 's/<[^>]*>//g;/</N;//ba')    # removes most html declarations to prevent XSS within documents
		#JSON=$(echo "$QUERY_STRING_POST" | jq .)        # json encode - This is a pretty save way to check for valide json code
		JSON=$(echo "$QUERY_STRING_POST") # line bevor gives an error even if valid json
    ;;
    *)
        response_with_html
        exit 0
    ;;
    esac

else
    response_with_html
    exit 0
fi

# Some Commands ...

response_with_json

exit 0