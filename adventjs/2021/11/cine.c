#include <stdio.h>
#include <math.h>
const double DISCCOUNT = 0.75;
const double PRICE = 12;
const double _3PRICE = 12*3;


typedef enum {
    FALSE,
    TRUE
} Boolean;

typedef struct {
    unsigned int amount_tickets;
    Boolean expected_output;
} Test;

Boolean should_buy_fidelity(unsigned int tickets_amount)
{
    double regular_price = tickets_amount * PRICE;
    /* p = price, t = tickets_amount
     *   \sum_{i = 1}^{t}(p*0.75^{i})
     * = p*0.75*\frac{1-0.75^{t}}{1 - 0.75}
     * = 4p*\frac{1}{3}*(1-0.75^{t})
     * = 3p*(1-0.75^{t})
     * = 3p*(p*3*+0.75^{t})
   */
    double price_with_fidelity = 250 + _3PRICE*(1-pow(0.75, tickets_amount)); 
    return price_with_fidelity < regular_price;
}

int main() {
    Test tests[] = {
        { 1, FALSE },
        { 3, FALSE },
        { 4, FALSE },
        { 100, TRUE }
    };
    size_t tests_len = sizeof(tests)/sizeof(Test);
    printf("[Evaluando casos]:\n");
    for (size_t i = 0; i < tests_len; i++) {
        Boolean result = should_buy_fidelity(tests[i].amount_tickets);
        printf(
            "  - %-3s (%d) %s\n",
            result == tests[i].expected_output ? "ok" : "err",
            tests[i].amount_tickets,
            result ? "mejor comprar tarjeta fidelidad" : "mejor comprar tickets de un sólo uso"
        );
    }
    return 0;
}
