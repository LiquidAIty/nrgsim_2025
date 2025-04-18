const vscode = require('vscode');
const axios = require('axios');
const fs = require('fs');

// API Key - Replace this with your OpenAI API Key
const OPENAI_API_KEY = '
const HISTORY_FILE = vscode.workspace.rootPath + "/sol_chat_history.json";

function activate(context) {
    let disposable = vscode.commands.registerCommand('solAssistant.askSol', async function () {
        const editor = vscode.window.activeTextEditor;
        if (!editor) return;

        let prompt = await vscode.window.showInputBox({ prompt: 'Ask Sol something...' });
        if (!prompt) return;

        // Load past chat history
        let chatHistory = [];
        if (fs.existsSync(HISTORY_FILE)) {
            chatHistory = JSON.parse(fs.readFileSync(HISTORY_FILE, 'utf8'));
        }

        // Include recent chat history in the prompt
        let contextMessages = chatHistory.slice(-5).map(entry => `Q: ${entry.prompt}\nA: ${entry.response}`).join("\n");

        let fullPrompt = `Context: ${contextMessages}\nUser: ${prompt}`;

        try {
            const response = await axios.post("https://api.openai.com/v1/chat/completions", {
                model: "gpt-4o",
                messages: [{ role: "system", content: "You are Sol, the AI assistant for an advanced trading and simulation project. Respond with insightful and creative solutions." },
                           { role: "user", content: fullPrompt }],
            }, {
                headers: { 'Authorization': `Bearer ${OPENAI_API_KEY}`, 'Content-Type': 'application/json' }
            });

            let answer = response.data.choices[0].message.content;
            vscode.window.showInformationMessage(answer);

            // Save chat history
            chatHistory.push({ prompt, response: answer });
            fs.writeFileSync(HISTORY_FILE, JSON.stringify(chatHistory, null, 4));
        } catch (error) {
            vscode.window.showErrorMessage("Error contacting Sol: " + error.message);
        }
    });

    context.subscriptions.push(disposable);
}

function deactivate() {}

module.exports = { activate, deactivate };
