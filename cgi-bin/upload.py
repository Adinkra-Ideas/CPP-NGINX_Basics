#!/usr/bin/python3

import cgi, os

form = cgi.FieldStorage()

# Get filename here
fileitem = form['filename']

# Test if the file was uploaded
if fileitem.filename:
   open(os.getcwd() + '/cgi-bin/uploads/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
   message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded to ' + os.getcwd() + '/cgi-bin/tmp'
else:
   message = 'Uploading Failed'

print("Content-Type: text/html;charset=utf-8")
print ("Content-type:text/html\r\n")
print("<style> html, body {justify-content: center;font-family: Roboto, Arial, sans-serif;font-size: 15px;} </style>")
print("<div class=\"container\"> ")
print("<H1> " + message + " </H1>")
print(" </div>")