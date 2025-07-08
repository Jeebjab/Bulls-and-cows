
        if (cin.fail()) {  // Handle rare input errors (e.g., Ctrl+Z)
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again: ";
            continue;
        }

        // Clear any extra characters (like if user typed "12345 678")
        cin.ignore(numeric_limits<streamsize>::max(), '\n');