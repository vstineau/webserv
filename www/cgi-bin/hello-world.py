#!/bin/python3
# import cgi
import sys

html_content = "<html><body><h1>Hello, CGI.py!</h1></body></html>"
content_length = len(html_content)
print(f"Content-Length: {content_length}")
print("Content-Type: text/html")
print()
print(html_content)