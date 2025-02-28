<h1 align="center">
<strong>AUT_AP_2025_Spring Homework 3</strong>
</h1>

<p align="center">
<strong> Deadline: 17th of Esfand - 23:59 o'clock</strong>
</p>

# **Insurance System Development**

This project is designed to simulate an insurance management system, focusing on core functionalities such as policy management, premium processing, claim handling, and risk assessment. By implementing the `Person`, `InsurancePolicy`, and `InsuranceCompany` classes, you will explore advanced object-oriented programming techniques, secure data handling, and robust error checking.

---

## Insurance Concepts Explained

-   **Premium:**  
    The premium is the fixed amount paid by the policyholder at regular intervals (e.g., monthly or annually) to keep the policy active.

-   **Claim:**  
    A claim is a formal request submitted by the policyholder to receive compensation for a loss or damage covered by the policy.

-   **Policy:**  
    An insurance policy is a contract between the customer and the insurance company. It details the coverage, premium amount, and terms under which claims will be handled.

-   **Risk Profile:**  
    The risk profile is a numerical indicator (from 1 to 10) that represents the customer’s risk level.
    **Adjustment Rules:**
    -   **Filing a Claim:** For every approved claim, increase the risk profile by 1, but do not exceed 10.
    -   **Timely Premium Payments:** For every three consecutive premium payments made on time without any claims, decrease the risk profile by 1, but do not go below 1.

---

## System Components

-   **Person Class**: Represents an individual with attributes like name, age, gender, and a risk profile. Learn to manage personal data securely using fingerprint hashing and validation.
-   **InsurancePolicy Class**: Models an insurance policy that links a customer to their coverage details. Each policy includes a unique policy number, policy type (Life, Health, Automobile, Property), premium amount, and coverage amount.
-   **InsuranceCompany Class**: Acts as the central entity that oversees policy issuance, premium collection, claim processing, and overall risk management. This class simulates real-world insurance operations by managing customers and policies via secure and efficient methods.

---

## Person Class Implementation Guide

The `Person` class is the foundation of our insurance system, representing an individual with personal details and a risk profile that affects their insurance premiums and claim processing.

### **Attributes and Requirements:**

-   **Attributes:**

    -   `const std::string name`: The customer’s full name.
    -   `size_t age`: The customer’s age.
    -   `const std::string gender`: Must be either `"Male"` or `"Female"`. Any other value must trigger a `std::invalid_argument` exception.
    -   `const size_t hashed_fingerprint`: A hashed value of the customer’s fingerprint, generated using `std::hash<std::string>`.
    -   `size_t risk_profile`: An integer from 1 (lowest risk) to 10 (highest risk). The initial value is provided in the constructor.
    -   `bool is_insured`: Indicates whether the customer currently holds an active policy.

-   **Implementation Rules:**
    -   **Spaceship Operator:** Implement the spaceship operator (`<=>`) to compare two `Person` objects **based solely on their `risk_profile`**.
    -   **Risk Profile Adjustment:** Follow the explicit rules given above. The logic for increasing or decreasing the risk profile must be integrated with claim processing and premium payments as described in the insurance concepts section.
    -   All invalid inputs must throw appropriate exceptions such as `std::invalid_argument`.

### Person Class Header

```cpp
#ifndef PERSON_H
#define PERSON_H

#include <compare>
#include <optional>
#include <string>
#include <stdexcept>

// Models an individual for insurance purposes.
class Person {
public:
    // Constructor with personal attributes.
    Person(const std::string &name, size_t age, const std::string &gender, const std::string &fingerprint, size_t risk_profile, bool is_insured);

    // Getters.
    std::string get_name() const;
    size_t get_age() const;
    std::string get_gender() const;
    size_t get_hashed_fingerprint() const;
    size_t get_risk_profile() const;
    bool get_is_insured() const;

    // Setters.
    bool set_age(size_t age);
    bool set_risk_profile(size_t risk_profile);
    bool set_is_insured(bool is_insured);

    // Spaceship operator: compare Persons by risk_profile.
    std::strong_ordering operator<=>(const Person &other) const;

    // Outputs person information; writes to file if file_name is provided.
    void get_info(std::optional<std::string> file_name = std::nullopt) const;

private:
    const std::string name;
    size_t age;
    const std::string gender;
    const size_t hashed_fingerprint;
    size_t risk_profile;
    bool is_insured;
};

#endif // PERSON_H
```

---

## **InsurancePolicy Class Implementation Guide**

The `InsurancePolicy` class encapsulates all details of an insurance policy, linking a customer to their coverage plan. This class is analogous to a bank account in a banking system, but it is tailored for insurance operations.

### **Attributes and Requirements:**

-   **Attributes:**

    -   `const Person* owner`: Pointer to the policy owner.
    -   `const InsuranceCompany* company`: Pointer to the issuing insurance company.
    -   `const std::string policy_number`: A unique 16-character identifier generated using `<random>`.
    -   `std::string policy_type`: Must be one of `"Life"`, `"Health"`, `"Automobile"`, or `"Property"`.
    -   `double premium_amount`: The fee charged for maintaining the policy.
    -   `double coverage_amount`: The maximum amount covered by the policy.
    -   `bool policy_status`: `true` if the policy is active; `false` if cancelled.

-   **Implementation Rules:**

    -   **Authentication:** Methods that access or modify sensitive policy data must verify the owner’s identity using a provided fingerprint (as a `const std::string&`).
    -   **Spaceship Operator:** Implement the spaceship operator to compare policies by their unique `policy_number`.
    -   **Unique Policy Number Generation:** Each policy must have a unique identifier (a randomly generated 16-character string). Utilize functions from `<random>` to ensure uniqueness.
    -   **Secure Access to Sensitive Information:** Sensitive details must be accessed only after verifying the owner’s fingerprint. Implement a verification mechanism that compares the hashed fingerprint provided with the stored hashed fingerprint.
    -   **Information Display:** The `get_info` method should output policy details in a clear, formatted manner. Exclude sensitive data from the output. If a file name is provided, write the details to a file; otherwise, display them on the terminal.
    -   All invalid inputs must throw appropriate exceptions such as `std::invalid_argument`.

### InsurancePolicy Header

```cpp
#ifndef INSURANCE_POLICY_H
#define INSURANCE_POLICY_H

#include <compare>
#include <optional>
#include <string>

class InsuranceCompany; // Forward declaration.
class Person;           // Forward declaration.

// Represents an insurance policy with coverage and premium details.
class InsurancePolicy {
    friend class InsuranceCompany; // Grant InsuranceCompany full access.

public:
    // Constructor with policy owner, company, and policy details.
    InsurancePolicy(const Person* const owner, const InsuranceCompany* const company, const std::string &policy_type, double premium_amount, double coverage_amount);

    // Getters.
    const Person* get_owner() const;
    std::string get_policy_number() const;
    std::string get_policy_type() const;
    double get_premium_amount() const;
    double get_coverage_amount() const;
    bool get_policy_status() const; // true if active, false if cancelled.

    // Authentication: Get policy details after verifying owner's fingerprint.
    std::string get_policy_details(const std::string &owner_fingerprint) const;

    // Setters with authentication.
    bool set_policy_type(const std::string &new_policy_type, const std::string &owner_fingerprint);
    bool set_premium_amount(double new_premium, const std::string &owner_fingerprint);

    // Spaceship operator: Compare policies by their unique policy_number.
    std::strong_ordering operator<=>(const InsurancePolicy &other) const;

    // Outputs policy information; writes to file if file_name is provided.
    void get_info(std::optional<std::string> file_name = std::nullopt) const;

private:
    const Person* owner;
    const InsuranceCompany* company;
    const std::string policy_number;
    std::string policy_type;
    double premium_amount;
    double coverage_amount;
    bool policy_status;
};

#endif // INSURANCE_POLICY_H
```

---

## **InsuranceCompany Class Implementation Guide**

The `InsuranceCompany` class is the central component of our insurance system, responsible for managing policies, processing premiums, handling claims, and assessing risk. This class simulates the operations of a real-world insurance firm.

### **Attributes and Requirements:**

-   **Attributes:**

    -   `const std::string company_name`: The name of the insurance company.
    -   `const size_t hashed_security_key`: A hashed version of the company’s security key (using `std::hash<std::string>`).
    -   `std::vector<Person*> customers`: A vector of pointers to customer objects.
    -   `std::vector<InsurancePolicy*> policies`: A vector of pointers to policy objects.
    -   `std::map<InsurancePolicy*, Person*> policy_to_customer`: A mapping from policies to their owners.
    -   `std::map<Person*, std::vector<InsurancePolicy*>> customer_to_policy`: A mapping from customers to their policies.
    -   `double total_premiums_collected`: Total premiums collected.
    -   `double total_claims_paid`: Total claims paid.

-   **Implementation Rules:**

    -   **Spaceship Operator:** When comparing two InsuranceCompany objects, compare them based on the number of customers.
    -   All invalid inputs must throw appropriate exceptions such as `std::invalid_argument`.

-   **Core Functionalities:**
    -   **Creating Policies:** Authenticate the customer with their fingerprint, generate a unique policy number, and create a new policy.
    -   **Canceling Policies:** Verify the customer’s identity before cancellation and update the records.
    -   **Processing Premium Payments:** Record payments, update policy histories, and adjust financial totals.
    -   **Filing Claims:** Validate a claim by comparing the claim amount with the policy’s coverage; then update financial statistics.
    -   **Risk Profile Adjustments:** Adjust the customer’s risk profile automatically using the rules detailed in the Insurance Concepts section.

### InsuranceCompany Header

```cpp
#ifndef INSURANCE_COMPANY_H
#define INSURANCE_COMPANY_H

#include <compare>
#include <map>
#include <optional>
#include <string>
#include <vector>

class InsurancePolicy; // Forward declaration.
class Person;          // Forward declaration.

// Represents an insurance company managing policies and customers.
class InsuranceCompany {
public:
    // Constructor with company name and security key.
    InsuranceCompany(const std::string &company_name, const std::string &security_key);

    // Destructor.
    ~InsuranceCompany();

    // Policy operations.
    InsurancePolicy* create_policy(Person &owner, const std::string &owner_fingerprint, const std::string &policy_type, double premium_amount, double coverage_amount);
    bool cancel_policy(InsurancePolicy &policy, const std::string &owner_fingerprint);
    bool process_premium(InsurancePolicy &policy, const std::string &owner_fingerprint, double amount);
    bool file_claim(InsurancePolicy &policy, const std::string &owner_fingerprint, double claim_amount);

    // Getters.
    const std::string& get_company_name() const;
    size_t get_hashed_security_key() const;

    // Methods that require authentication via the security key.
    const std::vector<Person*>& get_customers(const std::string &security_key) const;
    const std::vector<InsurancePolicy*>& get_policies(const std::string &security_key) const;
    const std::map<InsurancePolicy*, Person*>& get_policy_to_customer_map(const std::string &security_key) const;
    const std::map<Person*, std::vector<InsurancePolicy*>>& get_customer_to_policy_map(const std::string &security_key) const;
    double get_total_premiums_collected(const std::string &security_key) const;
    double get_total_claims_paid(const std::string &security_key) const;

    // Policy setter requiring authentication.
    bool set_policy_status(InsurancePolicy &policy, bool status, const std::string &security_key);

    // Spaceship operator: Compare companies by the number of customers.
    std::strong_ordering operator<=>(const InsuranceCompany &other) const;

    // Outputs company information; writes to file if file_name is provided.
    void get_info(std::optional<std::string> file_name = std::nullopt) const;

private:
    const std::string company_name;
    const size_t hashed_security_key;
    std::vector<Person*> customers;
    std::vector<InsurancePolicy*> policies;
    std::map<InsurancePolicy*, Person*> policy_to_customer;
    std::map<Person*, std::vector<InsurancePolicy*>> customer_to_policy;
    double total_premiums_collected;
    double total_claims_paid;
};

#endif // INSURANCE_COMPANY_H
```

## Utilizing `utils.cpp` for Common Functions

Throughout your implementation, you will encounter repeated tasks such as fingerprint hashing, random policy number generation, and premium/risk calculations. It is recommended that you consolidate these functions in a `utils.cpp` file.

### Benefits

-   **Reusability**: Write functions once and reuse them across multiple classes.
-   **Maintainability**: Centralizing common operations makes it easier to update your code in the future.
-   **Clarity**: Keeps your class implementations focused on their primary responsibilities by offloading repetitive tasks.

**_Remember to follow the DRY (Don't Repeat Yourself) principle!_**

## **Final Step: How To Test Your Program**

If you want to debug your code, set the `if` statement to `true`. This will allow you to place your debugging code in the designated section. Once you're done with the debugging process, remember to set the `if` statement back to `false` to test your program using the provided `unit-test.cpp`.

Furthermore, whatever code you write should be implemented in the `InsuranceCompany.cpp/h`, `InsurancePolicy.cpp/h`, `Person.cpp/h` and `utils.cpp` files. Please refrain from making any changes to other files in the project.

```cpp
#include <iostream>
#include <gtest/gtest.h>
#include "Person.h"
#include "InsurancePolicy.h"
#include "InsuranceCompany.h"

int main(int argc, char **argv)
{
    if (true) // Set to false to run unit-tests
    {
        // Debug section: Place your debugging code here
    }
    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}
```

<p align="center">
  <img src="./resource/goddam-patterns.webp" alt="Learn The goddam patterns" style="width: 60%;">
</p>

**Best Regards, [smhamidi](https://github.com/smhamidi)**
