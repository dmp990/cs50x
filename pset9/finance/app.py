import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, current_time

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Create necessary tables
db.execute("DROP TABLE IF EXISTS stock")
db.execute("DROP TABLE IF EXISTS history")
db.execute("CREATE TABLE stock (t_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER NOT NULL, symbol TEXT NOT NULL UNIQUE, shares NUMERIC NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id))")
db.execute("CREATE TABLE history (h_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER NOT NULL, symbol TEXT NOT NULL, price NUMERIC NOT NULL, shares NUMERIC NOT NULL, transacted TEXT, FOREIGN KEY(user_id) REFERENCES users(id))")


# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query the database to get every record of transaction with current_user
    rows = db.execute(
        "SELECT symbol, SUM(shares) as shares FROM stock WHERE user_id = ? GROUP BY symbol", session["user_id"])

    total_value = 0.0
    current_prices = {}
    for row in rows:
        price = lookup(row["symbol"])["price"]
        current_prices[row["symbol"]] = price
        total_value += (price * row["shares"])

    # Get the cash in account
    rows_n = db.execute(
        "SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash_remaining = rows_n[0]["cash"]
    cash_remaining = usd(cash_remaining)
    total_value = usd(total_value)

    return render_template("index.html", rows=rows, current_prices=current_prices, cash_remaining=cash_remaining, total_value=total_value, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Ensure that symbol was provided
        if not request.form.get("symbol"):
            return apology("Symbol not provided", 400)

        # Ensure that shares is provided and is a +ve int
        try:
            shares = int(request.form.get("shares"))
        except Exception:
            return apology("Shares must be a positive integer", 400)

        if shares < 0:
            return apology("Shares must be a positive number", 400)

        # Ensure that symbol is correct
        info_obj = lookup(request.form.get("symbol"))  # Name, price, symbol
        if info_obj == None:
            return apology("Symbol does not exist", 400)

        # submit the user info // buy the stocks
        symbol = request.form.get("symbol")

        current_price = info_obj["price"]
        rows = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash_in_account = rows[0]["cash"]
        total_needed_to_buy = current_price * shares
        print(cash_in_account, rows)

        # Ensure there is enough cash in account to buy
        if total_needed_to_buy > cash_in_account:
            return apology("Not enough cash in account", 400)

        # Enough cash, proceed to buy
        ret = db.execute("SELECT * FROM stock WHERE user_id = ? AND symbol = ?",
                         session["user_id"], symbol)
        if len(ret) == 0:
            db.execute("INSERT INTO stock (user_id, symbol, shares) VALUES (?, ?, ?)",
                       session["user_id"], symbol, shares)
        else:
            db.execute("UPDATE stock SET shares = ? WHERE symbol = ? AND user_id = ?",
                       ret[0]["shares"] + shares, symbol, session["user_id"])

        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   cash_in_account - total_needed_to_buy, session["user_id"])
        db.execute("INSERT INTO history (user_id, symbol, price, shares, transacted) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], symbol, current_price, shares, current_time())

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute(
        "SELECT * FROM history WHERE user_id = ?", session["user_id"])
    return render_template("history.html", rows=rows, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Get the symbol from post request
        symbol = request.form.get("symbol")

        # Look up the symbol
        info_obj = lookup(symbol)  # name, price, symbol

        if info_obj == None:
            return apology("Something went wrong", 400)

        return render_template("quoted.html", info_obj=info_obj, usd=usd)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation field was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Ensure password and confirmation match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # Check if the username already exists in the database
        rows = db.execute("SELECT * FROM users WHERE USERNAME = ?",
                          request.form.get("username"))
        if len(rows) != 0:
            return apology("username already exists", 400)

        # Store user info into the database
        db.execute("INSERT INTO USERS (username, hash) VALUES (?, ?)",
                   request.form.get("username"), generate_password_hash(request.form.get("password")))

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    if request.method == "POST":

        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must choose a symbol", 400)

        # Ensure that shares is provided and is a +ve int
        try:
            shares = int(request.form.get("shares"))
        except Exception:
            return apology("Shares must be a positive number", 400)

        if shares < 0:
            return apology("Shares must be a positive number", 400)

        # Check to see if user has enough shares to sell
        rows = db.execute("SELECT * FROM stock WHERE user_id = ? AND symbol = ?",
                          session["user_id"], symbol.lower())

        if len(rows) == 0:
            return apology("No Stocks", 400)

        if rows[0]["shares"] < shares:
            return apology("Do not have enough shares to sell", 400)

        # get the price
        info_obj = lookup(symbol)

        # Proceed to sell
        db.execute("UPDATE stock SET shares = shares - ? WHERE user_id = ? AND symbol = ?",
                   int(shares), session["user_id"], symbol.lower())
        db.execute("INSERT INTO history (user_id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, ?)", session["user_id"], symbol.lower(),
                   -1*shares, info_obj["price"], current_time())
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                   shares * info_obj["price"], session["user_id"])
        return redirect("/")

    else:

        rows = db.execute(
            "SELECT * FROM stock WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", rows=rows)
