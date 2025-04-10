
# 💰 Personal Finance Management System

This is a console-based **Personal Finance Management System** written in C++. The project was designed to help users manage their finances by tracking income, expenses, budgets, investments, savings goals, assets, liabilities, and more.

## 📌 Features

- **User Authentication**
  - Secure login and sign-up system
  - Unique user directories and files

- **Income Management**
  - Add, view, edit, and delete income
  - Calculate total income for a date range

- **Expense Tracking**
  - Add categorized expenses with budget checking
  - Edit or delete expenses
  - Calculate total expenses over time

- **Budget Planning**
  - Set monthly/yearly/none-type budgets
  - Alerts when nearing limits
  - Update or delete budgets

- **Investment Monitoring**
  - Track investment performance over time
  - Calculate ROI, current value, and total profit

- **Net Worth Calculation**
  - Manage and view assets and liabilities
  - Calculate net worth over time

- **Savings Goal**
  - Set savings goals with deadlines
  - Track progress and contribute to goals
  - Automatically prioritize goals

- **Recurring Transactions**
  - Handle repetitive financial transactions
  - Auto-update based on frequency and due date

- **Reminders**
  - Set up financial reminders (recurring or one-time)
  - Auto-remove or update reminders as due dates pass

- **Financial Reporting**
  - Generate detailed financial reports
  - Overview of income, expenses, investments, net worth, and summary

## 📁 Data Persistence

Each user has their own set of CSV files storing:
- Income
- Expenses
- Budgets
- Investments
- Assets
- Liabilities
- Goals
- Reminders
- Recurring transactions

This makes it easy to maintain separate records for multiple users.

## 🛠 Technologies Used

- **Language:** C++
- **Standard Library:** STL (maps, vectors, regex, etc.)
- **File Handling:** CSV for storage
- **No external dependencies**

## 🚀 How to Run

1. **Compile** the code with a C++ compiler (e.g., g++).
   ```bash
   g++ First_program.cpp -o FinanceApp
   ```

2. **Run** the application:
   ```bash
   ./FinanceApp
   ```

3. **Sign up or log in**, and begin managing your finances from the menu-driven interface.

## 🎯 Motivation

I created this project to learn advanced C++ concepts while building something practical and helpful. It encapsulates real-life financial scenarios and encourages better money management.

## 📦 Project Structure

```
├── First_program.cpp           # Main application logic
├── investment.h / expense.h    # Class definitions for finance components
├── users.csv                   # Credentials file
└── [username]_*.csv            # User-specific data files
```

## ✍️ Author

This project was developed and written by me around five months ago. If you have feedback or suggestions, feel free to reach out!

---

Let me know if you want me to tailor this for your GitHub theme or include images, usage examples, or a demo video.
