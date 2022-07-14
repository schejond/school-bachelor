from typing import List

from ExpertSystem.Business.UserFramework import IInference, ActionBaseCaller
from ExpertSystem.Structure.Enums import LogicalOperator, Operator
from ExpertSystem.Structure.RuleBase import Rule, Fact, ExpressionNode, Expression


class Trapezoid:
    def __init__(self, a: int, b: int, c: int, d: int):
        self.a = a
        self.b = b
        self.c = c
        self.d = d


class Inference(IInference):
    """
    | Method `interfere` will be called each turn or manually with `Inference` button.
    """
    knowledge_base: List[Fact]
    action_base: ActionBaseCaller

    def infere(self, knowledge_base: List[Fact], rules: List[Rule], action_base: ActionBaseCaller) -> None:
        """
        :param knowledge_base: - list of Fact classes defined in  KnowledgeBase.create_knowledge_base()
        :param rules:  - list of rules trees defined in rules file.
        :param action_base: - instance of ActionBaseCaller for executing conclusions
        """
        self.knowledge_base = knowledge_base
        self.action_base = action_base
        fuzzy_rules = []

        for rule in rules:
            condition = self.rule_evaluation(rule.condition)

            if type(condition) == int or type(condition) == float:
                condition = None

            fuzzy_value = self.fuzzy_evaluation(rule.condition)
            # print('fuzzy value:', fuzzy_value, 'cond', condition)

            # fuzzy rules will have None condition value but with fuzzy_value > 0
            if condition is None and fuzzy_value >= 0:
                # print(rule.conclusion.value.name, rule.conclusion.value.value, fuzzy_value)
                fuzzy_rules.append({'conclusion': rule.conclusion, 'fuzzy': fuzzy_value})
            # if conclusion is not fuzzy -> determine if the rule should be executed by fuzzy threshold
            elif condition:
                self.conclusion_evaluation(rule.conclusion)

        self.fuzzy_conclusion_evaluation(fuzzy_rules)

        # print('---------------------------------------------------------------------------------')

    # returns rule condition evaluation and uncertainty evaluation
    def rule_evaluation(self, root_node: ExpressionNode):
        if root_node.operator == LogicalOperator.AND:
            left_cond = self.rule_evaluation(root_node.left)
            right_cond = self.rule_evaluation(root_node.right)
            return left_cond and right_cond

        elif root_node.operator == LogicalOperator.OR:
            left_cond = self.rule_evaluation(root_node.left)
            right_cond = self.rule_evaluation(root_node.right)
            return left_cond or right_cond

        elif isinstance(root_node.value, Expression):
            try:
                evaluation = self.knowledge_base[self.knowledge_base.index(root_node.value.name)](*root_node.value.args)

                if root_node.value.comparator == Operator.MORE_THEN:
                    return evaluation > int(root_node.value.value)
                if root_node.value.comparator == Operator.LESS_THEN:
                    return evaluation < int(root_node.value.value)

                return evaluation

            except ValueError:
                return False

        else:
            return bool(root_node.value)

    # return fuzzy evaluation
    def get_trapezoid_set(self, name):
        money = {
            'low': Trapezoid(0, 0, 20, 25),
            'enough': Trapezoid(20, 25, 40, 45),
            'lot': Trapezoid(40, 45, 60, 65),
            'too_much': Trapezoid(60, 65, 500, 500)
        }
        base_health = {
            'low': Trapezoid(0, 0, 200, 225),
            'half': Trapezoid(200, 225, 350, 375),
            'full': Trapezoid(350, 375, 500, 500)
        }
        spawn_archers_fuzzy = {
            'few': Trapezoid(0, 0, 3, 4),
            'medium': Trapezoid(3, 4, 5, 6),
            'many': Trapezoid(5, 6, 7, 7)
        }
        closest_enemy_distance = {
            'close': Trapezoid(0, 0, 2, 3),
            'medium': Trapezoid(2, 3, 4, 5),
            'far_away': Trapezoid(4, 5, 10, 10)
        }

        fuzzy_sets = {
            'money': money,
            'base_health': base_health,
            'closest_enemy_distance': closest_enemy_distance,
            'spawn_archers_fuzzy': spawn_archers_fuzzy
        }
        return fuzzy_sets.get(name, None)

    def compute_fuzzy_value(self, trapezoid: Trapezoid, value: int):
        # fuzzy value uses different formulas depending on the shape of the trapezoid
        # without left side
        if trapezoid.a == trapezoid.b:
            if value <= trapezoid.c:
                return 1
            elif value >= trapezoid.d:
                return 0
            else:
                return (value - trapezoid.d)/(trapezoid.c - trapezoid.d)
        # without right side
        if trapezoid.c == trapezoid.d:
            if value <= trapezoid.a:
                return 0
            elif value >= trapezoid.b:
                return 1
            else:
                return (value - trapezoid.a)/(trapezoid.a - trapezoid.b)
        # full trapezoid
        return max(
            min(
                (value - trapezoid.a)/(trapezoid.b - trapezoid.a),
                1,
                (trapezoid.d - value)/(trapezoid.d - trapezoid.c)
            ),
            0
        )

    def fuzzy_evaluation(self, root_node: ExpressionNode):
        fuzzy_value = 1

        if root_node.operator == LogicalOperator.AND:
            left_fuzzy_value = self.fuzzy_evaluation(root_node.left)
            right_fuzzy_value = self.fuzzy_evaluation(root_node.right)
            return min(left_fuzzy_value, right_fuzzy_value)

        elif root_node.operator == LogicalOperator.OR:
            left_fuzzy_value = self.fuzzy_evaluation(root_node.left)
            right_fuzzy_value = self.fuzzy_evaluation(root_node.right)
            return max(left_fuzzy_value, right_fuzzy_value)

        elif isinstance(root_node.value, Expression):
            try:
                evaluation = self.knowledge_base[self.knowledge_base.index(root_node.value.name)](*root_node.value.args)
                if root_node.value.comparator == Operator.EQUAL:
                    trapezoid_set = self.get_trapezoid_set(root_node.value.name)
                    if trapezoid_set is not None and str(root_node.value.value) in trapezoid_set.keys():
                        trapezoid = trapezoid_set.get(str(root_node.value.value))
                        # print('eval', str(evaluation), type(evaluation))
                        # if type(evaluation) == int or type(evaluation) == float:
                        return self.compute_fuzzy_value(trapezoid, evaluation)
                        # else:
                        #     return 1
                    else:
                        return 0
                return fuzzy_value
            except ValueError:
                return 0
        else:
            return fuzzy_value

    def conclusion_evaluation(self, root_node: ExpressionNode):
        if self.action_base.has_method(root_node.value):
            self.action_base.call(root_node.value)

    def fuzzy_conclusion_evaluation(self, fuzzy_rules):
        # dict -> conclusion name (spawn_archers_fuzzy) : dict-conclusion argument (few): list of computed fuzzy values
        conclusion_to_type_to_fuzzys = {}
        for rule in fuzzy_rules:
            if rule.get('conclusion').value.name not in conclusion_to_type_to_fuzzys.keys():
                conclusion_to_type_to_fuzzys[rule.get('conclusion').value.name] = {
                    rule.get('conclusion').value.args[0]: [rule.get('fuzzy')]
                }
            elif rule.get('conclusion').value.args[0]\
                    not in conclusion_to_type_to_fuzzys.get(rule.get('conclusion').value.name).keys():
                conclusion_to_type_to_fuzzys\
                    .get(rule.get('conclusion').value.name)[rule.get('conclusion').value.args[0]] = [rule.get('fuzzy')]
            else:
                conclusion_to_type_to_fuzzys.get(rule.get('conclusion').value.name)\
                    .get(rule.get('conclusion').value.args[0]).append(rule.get('fuzzy'))
        # print('V dictionary je:', str(conclusion_to_type_to_fuzzys))

        # obtain maximum from each list of fuzzy values and replace the list with the max value
        for conclusionName in conclusion_to_type_to_fuzzys.keys():
            for conclusion_type in conclusion_to_type_to_fuzzys.get(conclusionName):
                max_val = max(conclusion_to_type_to_fuzzys.get(conclusionName).get(conclusion_type))
                conclusion_to_type_to_fuzzys.get(conclusionName)[conclusion_type] = max_val

        for conclusionName in conclusion_to_type_to_fuzzys.keys():
            sum_up = 0
            sum_down = 0
            trapezoid = self.get_trapezoid_set(conclusionName)  # get trapezoid for given conclusion name
            if trapezoid is None:
                continue

            # pro kazdej typ si vytahnu si vytahnu lichobeznik a jeho b a c a v intervalu b az c budu pricitat hodnotu
            # for each type which is present, obtain trapezoid and sum it's values according to formula
            for conclusion_type in conclusion_to_type_to_fuzzys.get(conclusionName):
                b = trapezoid.get(conclusion_type).b
                c = trapezoid.get(conclusion_type).c
                for i in range(b, c+1):
                    sum_up += (i * conclusion_to_type_to_fuzzys.get(conclusionName).get(conclusion_type))
                    sum_down += conclusion_to_type_to_fuzzys.get(conclusionName).get(conclusion_type)
            # print('sum_up', sum_up, 'sum_down', sum_down)
            if sum_down == 0:
                fuzz_sum = 0
            else:
                fuzz_sum = sum_up / sum_down
                fuzz_sum = round(fuzz_sum)
            # print('fuzz_sum', fuzz_sum)

            # find conclusion_node
            node = None
            for rule in fuzzy_rules:
                if rule.get('conclusion').value.name == conclusionName:
                    node = rule.get('conclusion')
                    break
            if node is not None and self.action_base.has_method(node.value):
                node.value.args = str(fuzz_sum)
                self.action_base.call(node.value)
