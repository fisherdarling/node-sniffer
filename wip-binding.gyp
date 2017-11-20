{
  "targets": [
    {
      "target_name": "sniffer",
      "sources": ["./src/addon.cpp"],
      "include_dirs": ["<!(node -e \"require('nan')\")", "./src/"],
      "link_settings": {
        "libraries": ["-lpcap"]
      }
    }
  ]
}
