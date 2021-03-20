//
// Created by Bittner on 07.03.2019.
//

#include <polypropylene/reflection/VariableRegister.h>

namespace PAX {
    VariableRegister ComposeVariableRegisters(const VariableRegister & a, const VariableRegister & b) {
        // Compose given variables with the predefined ones.
        // Therefore, copy the given VariableRegister, such that duplicates
        // are overriden with the custom variables.
        VariableRegister composedVariableRegister = a;
        for (const auto & bEntry : b) {
            composedVariableRegister.insert_or_assign(bEntry.first, bEntry.second);
        }
        return composedVariableRegister;
    }


    std::string VariableResolver::resolveVariables(const std::string & str, const VariableRegister & variables) {
        const std::size_t VariableDeclarationBeginLen = std::string(VariableDeclarationBegin).size();
        const std::size_t VariableDeclarationEndLen = std::string(VariableDeclarationEnd).size();

        std::string result = str;

        // str[varDecIndex] == VariableDeclaration
        std::size_t varDecIndex = result.find(VariableDeclarationBegin);
        while (varDecIndex != std::string::npos) {
            // finds first occurence and hence the end of our var
            std::size_t varDecEndIndex = result.find(VariableDeclarationEnd, varDecIndex);

            if (varDecEndIndex == std::string::npos) {
                PAX_LOG(Log::Level::Warn, "Variable declaration end missing in " << str.c_str());
                // terminate since we are left in an infinite loop otherwise
                break;
            } else {
                std::size_t varNameIndex  = varDecIndex + VariableDeclarationBeginLen;
                std::size_t varNamelength = varDecEndIndex - VariableDeclarationEndLen - varNameIndex + 1;
                std::string varName = result.substr(varNameIndex, varNamelength);
                std::string varValue;

                const auto & it = variables.find(varName);
                if (it != variables.end()) {
                    varValue = it->second;
                }

                result = result.replace(
                        varDecIndex,
                        varNamelength + VariableDeclarationBeginLen + VariableDeclarationEndLen,
                        varValue
                );
            }

            // search for next occurrence
            varDecIndex = result.find(VariableDeclarationBegin);
        }

        return result;
    }

    void VariableResolver::resolveAllInHierarchy(VariableHierarchy &vars, const VariableRegister &variables) {
        {
            VariableRegister::iterator it;
            for (it = vars.values.begin(); it != vars.values.end(); ++it) {
                std::string newFirst = resolveVariables(it->first, variables);
                std::string newSecond = resolveVariables(it->second, variables);
                vars.values[it->first] = newFirst;
                vars.values[it->second] = newSecond;
            }
            /*
            std::cout << "[VariableResolver::resolveAllInHierarchy] after resolve:" << std::endl;
            for (it = vars.values.begin(); it != vars.values.end(); ++it) {
                std::cout << "\t" << it->first.c_str() << " -> " << it->second.c_str() << std::endl;
            }//*/
        }
        {
            std::map<std::string, VariableHierarchy>::iterator it;
            for (it = vars.children.begin(); it != vars.children.end(); ++it) {
                resolveAllInHierarchy(it->second, variables);
            }
        }
    }
}
