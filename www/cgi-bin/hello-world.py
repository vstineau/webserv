import cgi

html_content = "<html><body><h1>Hello, CGI!</h1></body></html>"
content_length = len(html_content)
print("Content-Type: text/html")
print(f"Content-Length: {content_length}")
print()
print(html_content)