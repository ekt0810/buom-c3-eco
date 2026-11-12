import http.server,functools,pathlib,webbrowser
root=pathlib.Path(__file__).resolve().parent
handler=functools.partial(http.server.SimpleHTTPRequestHandler,directory=str(root))
server=http.server.ThreadingHTTPServer(('127.0.0.1',8765),handler)
print('Open http://127.0.0.1:8765/viewer/ — Ctrl+C to stop')
webbrowser.open('http://127.0.0.1:8765/viewer/')
try:server.serve_forever()
except KeyboardInterrupt:server.server_close()
