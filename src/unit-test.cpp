#include <gtest/gtest.h>

#include <fstream>
#include <functional>
#include <sstream>

#include "InsuranceCompany.h"
#include "InsurancePolicy.h"
#include "Person.h"

// ----------------------
// Person Tests
// ----------------------

TEST(PersonTest, Constructor_ValidInput_SetsAttributesCorrectly) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	EXPECT_EQ(p.get_name(), "John Doe");
	EXPECT_EQ(p.get_age(), 30);
	EXPECT_EQ(p.get_gender(), "Male");
	EXPECT_EQ(p.get_risk_profile(), 5);
	EXPECT_FALSE(p.get_is_insured());
	EXPECT_EQ(p.get_hashed_fingerprint(), std::hash<std::string>{}("fingerprint123"));
}

TEST(PersonTest, Constructor_InvalidGender_ThrowsInvalidArgument) {
	EXPECT_THROW(Person("Jane Doe", 25, "Other", "fingerprint456", 3, true), std::invalid_argument);
}

TEST(PersonTest, Setters_UpdateAttributesCorrectly) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	EXPECT_TRUE(p.set_age(31));
	EXPECT_EQ(p.get_age(), 31);
	EXPECT_TRUE(p.set_risk_profile(6));
	EXPECT_EQ(p.get_risk_profile(), 6);
	EXPECT_TRUE(p.set_is_insured(true));
	EXPECT_TRUE(p.get_is_insured());
}

TEST(PersonTest, Setters_InvalidRiskProfile_ReturnFalse) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	EXPECT_FALSE(p.set_risk_profile(11));  // Must be between 1 and 10
	EXPECT_EQ(p.get_risk_profile(), 5);
	EXPECT_FALSE(p.set_risk_profile(0));
	EXPECT_EQ(p.get_risk_profile(), 5);
}

TEST(PersonTest, SpaceshipOperator_ComparesRiskProfile) {
	Person p1("John Doe", 30, "Male", "fingerprint123", 5, false);
	Person p2("Jane Doe", 25, "Female", "fingerprint456", 3, true);
	EXPECT_TRUE((p2 <=> p1) < 0);  // Lower risk profile
	EXPECT_TRUE((p1 <=> p2) > 0);  // Higher risk profile
	Person p3("Jim Beam", 40, "Male", "fingerprint789", 5, false);
	EXPECT_TRUE((p1 <=> p3) == 0);	// Same risk profile
}

// ----------------------
// InsurancePolicy Tests
// ----------------------

TEST(InsurancePolicyTest, Constructor_SetsAttributesCorrectly) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy policy(&p, &company, "Life", 100.0, 10000.0);
	EXPECT_EQ(policy.get_owner(), &p);
	EXPECT_EQ(policy.get_policy_type(), "Life");
	EXPECT_EQ(policy.get_premium_amount(), 100.0);
	EXPECT_EQ(policy.get_coverage_amount(), 10000.0);
	EXPECT_TRUE(policy.get_policy_status());
	EXPECT_EQ(policy.get_policy_number().length(), 16);	// Unique 16-character policy number
}

TEST(InsurancePolicyTest, Constructor_InvalidPolicyType_ThrowsInvalidArgument) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	EXPECT_THROW(InsurancePolicy(&p, &company, "Invalid", 100.0, 10000.0), std::invalid_argument);
}

TEST(InsurancePolicyTest, Authentication_IncorrectFingerprint_DeniesAccess) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy policy(&p, &company, "Life", 100.0, 10000.0);
	EXPECT_THROW(policy.get_policy_details("wrongfingerprint"), std::invalid_argument);
}

TEST(InsurancePolicyTest, Setters_UpdateAttributesAfterAuthentication) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy policy(&p, &company, "Life", 100.0, 10000.0);
	EXPECT_TRUE(policy.set_policy_type("Health", "fingerprint123"));
	EXPECT_EQ(policy.get_policy_type(), "Health");
	EXPECT_TRUE(policy.set_premium_amount(150.0, "fingerprint123"));
	EXPECT_EQ(policy.get_premium_amount(), 150.0);
}

TEST(InsurancePolicyTest, Setters_IncorrectFingerprint_DoNotUpdate) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy policy(&p, &company, "Life", 100.0, 10000.0);
	EXPECT_FALSE(policy.set_policy_type("Health", "wrongfingerprint"));
	EXPECT_EQ(policy.get_policy_type(), "Life");
}

TEST(InsurancePolicyTest, SpaceshipOperator_ComparesPolicyNumber) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy policy1(&p, &company, "Life", 100.0, 10000.0);
	InsurancePolicy policy2(&p, &company, "Health", 150.0, 20000.0);
	EXPECT_TRUE((policy1 <=> policy2) != 0);  // Different policy numbers
}

// ----------------------
// InsuranceCompany Tests
// ----------------------

TEST(InsuranceCompanyTest, Constructor_SetsCompanyNameAndHashedKey) {
	InsuranceCompany company("SafeLife", "security123");
	EXPECT_EQ(company.get_company_name(), "SafeLife");
	EXPECT_EQ(company.get_hashed_security_key(), std::hash<std::string>{}("security123"));
}

TEST(InsuranceCompanyTest, CreatePolicy_ValidInput_CreatesPolicy) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	ASSERT_NE(policy, nullptr);
	EXPECT_EQ(policy->get_policy_type(), "Life");
	EXPECT_TRUE(p.get_is_insured());
	EXPECT_EQ(company.get_policies("security123").size(), 1);
}

TEST(InsuranceCompanyTest, CreatePolicy_InvalidFingerprint_DoesNotCreatePolicy) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	EXPECT_THROW(company.create_policy(p, "wrongfingerprint", "Life", 100.0, 10000.0), std::invalid_argument);
	EXPECT_FALSE(p.get_is_insured());
	EXPECT_EQ(company.get_policies("security123").size(), 0);
}

TEST(InsuranceCompanyTest, CancelPolicy_ValidInput_CancelsPolicy) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	EXPECT_TRUE(company.cancel_policy(*policy, "fingerprint123"));
	EXPECT_FALSE(policy->get_policy_status());
}

TEST(InsuranceCompanyTest, CancelPolicy_InvalidFingerprint_DoesNotCancel) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	EXPECT_FALSE(company.cancel_policy(*policy, "wrongfingerprint"));
	EXPECT_TRUE(policy->get_policy_status());
}

TEST(InsuranceCompanyTest, ProcessPremium_ValidInput_UpdatesTotals) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	EXPECT_TRUE(company.process_premium(*policy, "fingerprint123", 100.0));
	EXPECT_EQ(company.get_total_premiums_collected("security123"), 100.0);
}

TEST(InsuranceCompanyTest, FileClaim_ValidInput_ProcessesClaim) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	EXPECT_TRUE(company.file_claim(*policy, "fingerprint123", 5000.0));
	EXPECT_EQ(company.get_total_claims_paid("security123"), 5000.0);
	EXPECT_EQ(p.get_risk_profile(), 6);	 // Risk profile increases by 1
}

TEST(InsuranceCompanyTest, FileClaim_ExceedsCoverage_DoesNotProcess) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	EXPECT_FALSE(company.file_claim(*policy, "fingerprint123", 15000.0));
	EXPECT_EQ(company.get_total_claims_paid("security123"), 0.0);
	EXPECT_EQ(p.get_risk_profile(), 5);	 // Risk profile unchanged
}

TEST(InsuranceCompanyTest, RiskProfileAdjustment_AfterThreePremiums) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	company.process_premium(*policy, "fingerprint123", 100.0);
	company.process_premium(*policy, "fingerprint123", 100.0);
	company.process_premium(*policy, "fingerprint123", 100.0);
	EXPECT_EQ(p.get_risk_profile(), 4);  // Decreases by 1 after 3 consecutive premiums without claims
}

TEST(InsuranceCompanyTest, RiskProfileAdjustment_AfterClaimAndPremiums) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	company.file_claim(*policy, "fingerprint123", 5000.0);	// Risk profile -> 6
	company.process_premium(*policy, "fingerprint123", 100.0);
	company.process_premium(*policy, "fingerprint123", 100.0);
	company.process_premium(*policy, "fingerprint123", 100.0);
	EXPECT_EQ(p.get_risk_profile(), 5);  // Decreases by 1 after 3 consecutive premiums post-claim
}

TEST(InsuranceCompanyTest, Getters_AfterAuthentication_ReturnCorrectData) {
	InsuranceCompany company("SafeLife", "security123");
	EXPECT_NO_THROW(company.get_customers("security123"));
	EXPECT_THROW(company.get_customers("wrongkey"), std::invalid_argument);
	EXPECT_EQ(company.get_total_premiums_collected("security123"), 0.0);
}

TEST(InsuranceCompanyTest, SpaceshipOperator_ComparesNumberOfCustomers) {
	InsuranceCompany company1("SafeLife", "security123");
	InsuranceCompany company2("SecureInsure", "security456");
	Person p1("John Doe", 30, "Male", "fingerprint123", 5, false);
	Person p2("Jane Doe", 25, "Female", "fingerprint456", 3, false);
	company1.create_policy(p1, "fingerprint123", "Life", 100.0, 10000.0);
	company2.create_policy(p1, "fingerprint123", "Health", 150.0, 20000.0);
	company2.create_policy(p2, "fingerprint456", "Automobile", 200.0, 15000.0);
	EXPECT_TRUE((company1 <=> company2) < 0);	 // company1: 1 customer, company2: 2 customers
}

TEST(InsuranceCompanyTest, PolicyNumberUniqueness) {
	Person p("John Doe", 30, "Male", "fingerprint123", 5, false);
	InsuranceCompany company("SafeLife", "security123");
	InsurancePolicy* policy1 = company.create_policy(p, "fingerprint123", "Life", 100.0, 10000.0);
	InsurancePolicy* policy2 = company.create_policy(p, "fingerprint123", "Health", 150.0, 20000.0);
	EXPECT_NE(policy1->get_policy_number(), policy2->get_policy_number());
}
