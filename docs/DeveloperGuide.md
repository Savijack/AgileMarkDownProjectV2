# Contributing Guidelines

This document outlines the workflow for our team. Please follow all steps below to maintain a clean repository and streamlined development process.

## Contribution Workflow

### Step 1: Find an Issue/Story to Work On
Find an issue or story to work on from the project backlog. If you want to make a change that doesn't have a story, create one first.

### Step 2: Create Feature Branch
Create a feature branch off of `main`:

```bash
git checkout main
git pull origin main
git checkout -b feature/your-feature-name
```

### Step 3: Write Tests
Write tests that fail initially, but will pass when your feature is implemented. Follow the test-driven development (TDD) approach.

#### How To Run Tests

1. Create a MarkDown file containing the MarkDown syntax that will be tested.
2. Use a separate .cpp file to perform tests.
3. Ensure test cases cover single and multiple instances of MarkDown syntax.
4. Test cases should have formatting similar to the following example:
```
TEST_CASE("Convert Italics Test Cases")
{
    HTMLConverter * test = new HTMLConverter("./test_documents/1.md");
    {
        string s = "*hello*";
        test->convertItalics(s);
        REQUIRE(s == "<em>hello</em>");
    }

    {
        string s = "this is *italic* text";
        test->convertItalics(s);
        REQUIRE(s == "this is <em>italic</em> text");
    }

    {
        string s = "*one* *two*";
        test->convertItalics(s);
        REQUIRE(s == "<em>one</em> <em>two</em>");
    }
}
```

### Step 4: Implement Feature
Implement the feature or make the required changes to satisfy the tests and requirements.

### Step 5: Push Changes and Create Pull Request

***PULL MAIN BEFORE PUSHING CHANGES***

After pulling the most recent code, push your changes to the feature branch and create a pull request to merge to `main`:
```bash
git push origin feature/your-feature-name
```
- Put your PR link in the story which it is connected to
- List the story number/ID in the PR description

### Step 6: Request Review
Request a review from the team. If the PR is not approved, address the feedback, repeat the previous steps, and resubmit the PR.

### Step 7: Merge and Close
If approved, merge the PR to `main` and close the associated story.
