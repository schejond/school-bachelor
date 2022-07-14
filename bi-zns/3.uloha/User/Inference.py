from typing import List

from ExpertSystem.Business.UserFramework import IInference, ActionBaseCaller
from ExpertSystem.Structure.Enums import LogicalOperator, Operator
from ExpertSystem.Structure.RuleBase import Rule, Fact, ExpressionNode, Expression


class Inference(IInference):
    """
    | User definition of the inference. You can define here you inference method (forward or backward).
      You can have here as many functions as you want, but you must implement interfere with same signature

    |
    | `def interfere(self, knowledge_base: List[Fact], rules: List[Rule], action_base: ActionBase):`
    |

    | Method `interfere` will be called each turn or manually with `Inference` button.
    | Class have no class parameters, you can use only inference parameters

    """
    knowledge_base: List[Fact]
    action_base: ActionBaseCaller


    def infere(self, knowledge_base: List[Fact], rules: List[Rule], action_base: ActionBaseCaller) -> None:
        """
        User defined inference

        :param knowledge_base: - list of Fact classes defined in  KnowledgeBase.create_knowledge_base()
        :param rules:  - list of rules trees defined in rules file.
        :param action_base: - instance of ActionBaseCaller for executing conclusions
        """
        self.knowledge_base = knowledge_base
        self.action_base = action_base

        for rule in rules:
            condition, uncertainty = self.rule_evaluation(rule.condition)

            if rule.uncertainty:
                uncertainty = uncertainty * rule.uncertainty

            uncertainty = round(uncertainty, 3)

            # if condition:
                # print(f'pravidlo je splneno:{rule}')
                # print('uncertainty je: ' + str(uncertainty))

            if condition and uncertainty >= 0.3:
                self.conclusion_evaluation(rule.conclusion)

            # if condition:
            #     if uncertainty < self.UNCERTAINTY_THRESHOLD:
            #         for other_rule in rules:
            #             if other_rule.uncertainty and str(other_rule.conclusion) == str(rule.conclusion) \
            #                     and str(rule.condition) != str(other_rule.condition):
            #                 tmp = other_rule.uncertainty + uncertainty - other_rule.uncertainty * uncertainty
            #                 other_rule.uncertainty = tmp
            #
            #     else:
            #         print(uncertainty)
            #         self.conclusion_evaluation(rule.conclusion)
        # print('---------------------------------------------------------------------------------')


    # returns rule condition evaluation and uncertainty evaluation
    def rule_evaluation(self, root_node: ExpressionNode):
        uncertainty = 1

        if root_node.operator == LogicalOperator.AND:
            left_cond, left_uncertainty = self.rule_evaluation(root_node.left)
            right_cond, right_uncertainty = self.rule_evaluation(root_node.right)
            return left_cond and right_cond, min(left_uncertainty, right_uncertainty)

        elif root_node.operator == LogicalOperator.OR:
            left_cond, left_uncertainty = self.rule_evaluation(root_node.left)
            right_cond, right_uncertainty = self.rule_evaluation(root_node.right)
            return self.return_or_values(left_cond, right_cond, left_uncertainty, right_uncertainty)

        elif isinstance(root_node.value, Expression):
            try:
                evaluation = self.knowledge_base[self.knowledge_base.index(root_node.value.name)](*root_node.value.args)
                uncertainty *= self.knowledge_base[self.knowledge_base.index(root_node.value.name)].probability

                if root_node.value.uncertainty:
                    uncertainty *= root_node.value.uncertainty
                    #print(f'Expression {root_node} with uncertainty {uncertainty}')

                if root_node.value.comparator == Operator.MORE_THEN:
                    return evaluation > int(root_node.value.value), uncertainty
                if root_node.value.comparator == Operator.LESS_THEN:
                    return evaluation < int(root_node.value.value), uncertainty

                return evaluation, uncertainty

            except ValueError:
                return False, uncertainty

        else:
            return bool(root_node.value), uncertainty

    def return_or_values(self, left_cond, right_cond, left_uncertainty, right_uncertainty):
        if left_cond and not right_cond:
            return True, left_uncertainty
        elif not left_cond and right_cond:
            return True, right_uncertainty
        else:
            return left_cond or right_cond, max(left_uncertainty, right_uncertainty)

    def conclusion_evaluation(self, root_node: ExpressionNode):
        if self.action_base.has_method(root_node.value):
            self.action_base.call(root_node.value)
