const transactionsBody = document.getElementById("transactions-body");
const errorEl = document.getElementById("error-message");
const clearBtn = document.getElementById("clear-all");
document.addEventListener("DOMContentLoaded", loadTransactions);
console.log("JS loaded")
function loadTransactions() {
    fetch("/transactions")
        .then(response => response.json())
        .then(transactions => {
            // Loop through each transaction and create a table row
            transactions.forEach(t => {
                const row = document.createElement("tr"); // Create a new row
                row.innerHTML = `
                    <td>${t.date}</td>
                    <td>${t.description}</td>
                    <td>${t.amount}</td>
                    <td>${t.type}</td>
                `;
                transactionsBody.appendChild(row); // Add the row to the table
            });
        })
        .catch(error => console.error("Error fetching transactions:", error));
}

const transactionForm = document.getElementById("transaction-form");

function isValidDate(dateString) {
    // Check format: MM-DD-YYYY
    const regex = /^(0[1-9]|1[0-2])-(0[1-9]|[0-2][0-9]|3[01])-\d{4}$/;
    if (!regex.test(dateString)) return false;

    // Parse components
    const [month, day, year] = dateString.split("-").map(Number);

    // Create a Date object (month is 0-indexed in JS)
    const date = new Date(year, month - 1, day);

    // Check if Date object actually matches input (catches invalid dates like 02-30-2025)
    return (
        date.getFullYear() === year &&
        date.getMonth() === month - 1 &&
        date.getDate() === day
    );
}

transactionForm.addEventListener("submit", function(event) {
    event.preventDefault();
    const description = document.getElementById("description").value;
    const amount = parseFloat(document.getElementById("amount").value);
    const date = document.getElementById("date").value;
    const type = document.getElementById("type").value;
    if (!description || !amount || !date || !type) {
        errorEl.textContent = "All fields are required.";
        return;
    }

    if (isNaN(amount) || amount <= 0) {
        errorEl.textContent = "Amount must be a positive number.";
        return;
    }

    if (!isValidDate(date)) {
        errorEl.textContent = "Please enter a valid date in MM-DD-YYYY format.";
        return;
    }

    if (type !== "Income" && type !== "Expense") {
        errorEl.textContent = "Transaction type must be Income or Expense.";
        return;
    }
    const newTransaction = {
        description,
        amount,
        date,
        type
    };
    fetch("/newtransaction", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(newTransaction)
    })
        .then(response => {
            if (response.ok) {
                const row = document.createElement("tr");
                row.innerHTML = `
                    <td>${newTransaction.date}</td>
                    <td>${newTransaction.description}</td>
                    <td>${newTransaction.amount}</td>
                    <td>${newTransaction.type}</td>
                `;
                transactionsBody.appendChild(row);
            } else {
                console.error("Failed to add transaction");
            }

        })
        .catch(error => console.error("Error adding transaction:", error));
});

clearBtn.addEventListener("click", () => {
    fetch("/cleartransactions", { method: "POST" })
        .then(response => response.text())
        .then(msg => {
            console.log(msg);
            // Clear the table visually
            transactionsBody.innerHTML = "";
        })
        .catch(err => console.error("Error clearing transactions:", err));

});
