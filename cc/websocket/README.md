# WebSocket

> https://developer.mozilla.org/zh-CN/docs/Web/API/WebSockets_API/Writing_WebSocket_servers

A simple websoskcet server.

- Depends on libssl and libcrypto, use SHA1.
- Use 3-party log.c lib.
- Be different in 3-times-shake of socket connection.
- Read frame buff need dynamic plan, because the length of net frame is variable.
