{
  "targets": [
    {
      "target_name": "addon",
      "sources": ["./src/addon.cpp"],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
