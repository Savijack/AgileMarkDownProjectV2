# C++ Starter

## Getting Started

### Option A: Codespaces in Browser

1. Click the green "Code" button
2. Select "Codespaces" tab
3. Click "Create codespace on main"
4. Wait for the environment to build

### Option B: Local VS Code + Codespaces

Use your local VS Code as the editor while running code in the cloud.

1. Install the "GitHub Codespaces" extension in VS Code
2. Sign in to GitHub within VS Code (click the Accounts icon in the sidebar)
3. Create a codespace from the GitHub website (Option A steps 1-3)
4. In VS Code: Ctrl+Shift+P → "Codespaces: Connect to Codespace"
5. Select your codespace

You now have local VS Code connected to the cloud environment. All your local themes and keybindings work, but code runs on the Codespace.

**Note:** The Codespace VM is still running when connected this way. Stop it when you're done to conserve hours.

## Running Your Code

**Option A: Terminal**

```bash
g++ -g -Wall -o program *.cpp
./program
```

**Option B: Press F5 to build and run with debugging**

## Debugging

1. Click in the gutter (left of line numbers) to set a breakpoint
2. Press F5
3. Use the debug toolbar to step through your code

## Project Structure

```
your-repo/
├── .devcontainer/
│   └── devcontainer.json    # Codespaces environment config
├── .vscode/
│   ├── launch.json          # Debug configuration
│   └── tasks.json           # Build task
├── main.cpp                 # Entry point
├── helper.h                 # Example header
├── helper.cpp               # Example implementation
└── README.md
```

## Adding New Files

Just create new `.cpp` and `.h` files. The build task compiles all `.cpp` files automatically.

## Common Issues

**Breakpoints not working?**

Make sure you're pressing F5 (Debug) not just running in the terminal. The `-g` flag is required for debugging.

**Compiler errors not showing?**

Try Ctrl+Shift+B to build manually. Errors appear in the Terminal panel.

## Managing Codespace Hours

Free GitHub accounts get 120 core-hours/month (about 60 hours on a 2-core machine).

**To stop your Codespace:**

- In browser: Click "Codespaces" in the bottom-left → "Stop Codespace"
- In local VS Code: Ctrl+Shift+P → "Codespaces: Stop Current Codespace"
- Or let it auto-stop after 30 minutes of inactivity

Your work is saved. Restarting takes about 30 seconds.
