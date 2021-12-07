# logos-format
Currently under testing phase

- This uses clang-format to format logos files, therefore you can pass in any argument you would normally pass to clang-format

- This requires a .clang-format file to be present in your current project directory, clang-format will automatically find the clang-format file if it is present in your project structure.

- To work correctly, it also requires the following option to be set to 0 in your .clang-format file <br>
      ```
      ContinuationIndentWidth: 0
      ```

An example .clang-format file would look something like this, I picked the style chromium because of their extensive docs


```
BasedOnStyle: Chromium
ContinuationIndentWidth: 0
```


Check out more about Chromium Objective-C style guide here https://google.github.io/styleguide/objcguide.html




<h2>How to use</h2>

- clang-format is required to be installed on your machine <br>
On Macs: 

    ```
    brew install clang-format
    ```
- python3 is required to be installed on your machine <br>
On Macs: 

    ```
    brew install python
    ```
 - Download logos-format.py and place in desired directory <br>
`curl -JLO https://github.com/HearseDev/logos-format/raw/main/logos-format.py` <br>


<h3>To use it with CLI</h3>

```
python3 /path/to/logos-format.py --assume-filename objc < /path/to/Tweak.xm
```

<h3>To use it with VSCode</h3>
For now this is just a method for testing <br>

- You need to have the logos extension by aarnav installed


- Install External formatters from the marketplace
https://marketplace.visualstudio.com/items?itemName=SteefH.external-formatters

- Add this code to your settings.json
```
"externalFormatters.languages": {
    "logos": {
        "command": "python3",
        "arguments": [
            "/path/to/logos-format.py",
            "--assume-filename",
            "objc",
        ]
    },
}
```
- You can format using the default VSCode option to format, you can have it format on save as well


<h3>To use it with neovim</h3>

- Install this plugin https://github.com/mhartington/formatter.nvim

- Add this to your init.vim
```
autocmd BufNewFile,BufRead *.xm,*.x :set filetype=logos
nnoremap <silent> <space>f :Format<CR>
lua << EOF
require('formatter').setup {
  filetype = {
    logos = {
      -- clang-format
      function()
        return {
          --change this
          exe = 'python3',
          args = {"/path/to/logos-format.py","--assume-filename", "objc"},
          stdin = true,
          cwd = vim.fn.expand '%:p:h',  -- Run clang-format in cwd of the file.
        }
      end,
    },
  },
}
EOF
```
- Now you can format the file using your keybind, in this case, using space + f
  
<h3>To use with others</h3>

- Logos-format works with any text editor that allows the use of external formatters. This is because logos-format works like any traditional formatter, it reads input from stdin and spits out formatted code to stdout. Just look up how to set up an external formatter for your text editor. I will be adding popular options here once i have finished testing.
